#include "codeeditor.h"

#include <KSyntaxHighlighting/Definition>
#include <KSyntaxHighlighting/FoldingRegion>
#include <KSyntaxHighlighting/SyntaxHighlighter>
#include <KSyntaxHighlighting/Theme>

#include <QActionGroup>
#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QFontDatabase>
#include <QMenu>
#include <QPainter>
#include <QPalette>

class SegmentData : public QTextBlockUserData
{
    Diff::Segment *_segment{nullptr};

public:
    SegmentData(Diff::Segment *segment) : _segment{segment} {}
    Diff::Segment *segment() const;
    void setSegment(Diff::Segment *newSegment);
};

Diff::Segment *SegmentData::segment() const
{
    return _segment;
}

void SegmentData::setSegment(Diff::Segment *newSegment)
{
    _segment = newSegment;
}

class CodeEditorSidebar : public QWidget
{
    Q_OBJECT
public:
    explicit CodeEditorSidebar(CodeEditor *editor);
    QSize sizeHint() const override;

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    CodeEditor *m_codeEditor;
};

CodeEditorSidebar::CodeEditorSidebar(CodeEditor *editor) : QWidget(editor), m_codeEditor(editor) {}

QSize CodeEditorSidebar::sizeHint() const
{
    return QSize(m_codeEditor->sidebarWidth(), 0);
}

void CodeEditorSidebar::paintEvent(QPaintEvent *event)
{
    m_codeEditor->sidebarPaintEvent(event);
}

void CodeEditorSidebar::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->x() >= width() - m_codeEditor->fontMetrics().lineSpacing()) {
        auto block = m_codeEditor->blockAtPosition(event->y());
        if (!block.isValid() || !m_codeEditor->isFoldable(block))
            return;
        m_codeEditor->toggleFold(block);
    }
    QWidget::mouseReleaseEvent(event);
}

CodeEditor::CodeEditor(QWidget *parent)
    : QPlainTextEdit(parent)
          , m_highlighter(new KSyntaxHighlighting::SyntaxHighlighter(document()))
      ,
      m_sideBar(new CodeEditorSidebar(this))
{
    setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
    setWordWrapMode(QTextOption::NoWrap);

    setTheme((palette().color(QPalette::Base).lightness() < 128)
                 ? m_repository.defaultTheme(KSyntaxHighlighting::Repository::DarkTheme)
                 : m_repository.defaultTheme(KSyntaxHighlighting::Repository::LightTheme));

    connect(this, &QPlainTextEdit::blockCountChanged, this, &CodeEditor::updateSidebarGeometry);
    connect(this, &QPlainTextEdit::updateRequest, this, &CodeEditor::updateSidebarArea);
    connect(this, &QPlainTextEdit::cursorPositionChanged, this, &CodeEditor::highlightCurrentLine);

    updateSidebarGeometry();
    highlightCurrentLine();

    QTextBlockFormat normalFormat;
    QTextBlockFormat addedFormat;
    QTextBlockFormat removedFormat;
    QTextBlockFormat changedFormat;

    addedFormat.setBackground(Qt::green);
    removedFormat.setBackground(Qt::red);
    changedFormat.setBackground(Qt::cyan);
    //    normalFormat.setBackground(Qt::lightGray);

    _formats.insert(Added, addedFormat);
    _formats.insert(Removed, removedFormat);
    _formats.insert(Unchanged, normalFormat);
    _formats.insert(Edited, changedFormat);
}

CodeEditor::~CodeEditor() {}

void CodeEditor::resizeEvent(QResizeEvent *event)
{
    QPlainTextEdit::resizeEvent(event);
    updateSidebarGeometry();
}

void CodeEditor::mousePressEvent(QMouseEvent *event)
{
    QPlainTextEdit::mousePressEvent(event);
}

void CodeEditor::setTheme(const KSyntaxHighlighting::Theme &theme)
{
    auto pal = qApp->palette();
    if (theme.isValid()) {
        pal.setColor(QPalette::Base, theme.editorColor(KSyntaxHighlighting::Theme::BackgroundColor));
        pal.setColor(QPalette::Highlight,
                     theme.editorColor(KSyntaxHighlighting::Theme::TextSelection));
    }
    setPalette(pal);

    m_highlighter->setTheme(theme);
    m_highlighter->rehighlight();
    highlightCurrentLine();
}

int CodeEditor::sidebarWidth() const
{
    int digits = 1;
    auto count = blockCount();
    while (count >= 10) {
        ++digits;
        count /= 10;
    }
    //return 4 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits + fontMetrics().lineSpacing();
    return 4 + fontMetrics().width(QLatin1Char('9')) * digits + fontMetrics().lineSpacing();
}

void CodeEditor::sidebarPaintEvent(QPaintEvent *event)
{
    QPainter painter(m_sideBar);
    painter.fillRect(event->rect(),
                     m_highlighter->theme().editorColor(KSyntaxHighlighting::Theme::IconBorder));

    auto block = firstVisibleBlock();
    auto blockNumber = block.blockNumber();
    int top = blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + blockBoundingRect(block).height();
    const int currentBlockNumber = textCursor().blockNumber();

    const auto foldingMarkerSize = fontMetrics().lineSpacing();

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            const auto number = QString::number(blockNumber + 1);
            painter.fillRect(QRect{0,
                                   top,
                                   m_sideBar->width() - 1,
                                   fontMetrics().height()},
                             document()->findBlockByNumber(blockNumber).blockFormat().background());

            painter.setPen(m_highlighter->theme().editorColor(
                (blockNumber == currentBlockNumber) ? KSyntaxHighlighting::Theme::CurrentLineNumber
                                                    : KSyntaxHighlighting::Theme::LineNumbers));
            painter.drawText(0,
                             top,
                             m_sideBar->width() - 2 - foldingMarkerSize,
                             fontMetrics().height(),
                             Qt::AlignRight,
                             number);

        }

        // folding marker
        if (block.isVisible() && isFoldable(block)) {
            QPolygonF polygon;
            if (isFolded(block)) {
                polygon << QPointF(foldingMarkerSize * 0.4, foldingMarkerSize * 0.25);
                polygon << QPointF(foldingMarkerSize * 0.4, foldingMarkerSize * 0.75);
                polygon << QPointF(foldingMarkerSize * 0.8, foldingMarkerSize * 0.5);
            } else {
                polygon << QPointF(foldingMarkerSize * 0.25, foldingMarkerSize * 0.4);
                polygon << QPointF(foldingMarkerSize * 0.75, foldingMarkerSize * 0.4);
                polygon << QPointF(foldingMarkerSize * 0.5, foldingMarkerSize * 0.8);
            }
            painter.save();
            painter.setRenderHint(QPainter::Antialiasing);
            painter.setPen(Qt::NoPen);
            painter.setBrush(QColor(
                m_highlighter->theme().editorColor(KSyntaxHighlighting::Theme::CodeFolding)));
            painter.translate(m_sideBar->width() - foldingMarkerSize, top);
            painter.drawPolygon(polygon);
            painter.restore();
        }

        block = block.next();
        top = bottom;
        bottom = top + blockBoundingRect(block).height();
        ++blockNumber;
    }
}

void CodeEditor::updateSidebarGeometry()
{
    setViewportMargins(sidebarWidth(), 0, 0, 0);
    const auto r = contentsRect();
    m_sideBar->setGeometry(QRect(r.left(), r.top(), sidebarWidth(), r.height()));
}

void CodeEditor::updateSidebarArea(const QRect &rect, int dy)
{
    if (dy)
        m_sideBar->scroll(0, dy);
    else
        m_sideBar->update(0, rect.y(), m_sideBar->width(), rect.height());
}

void CodeEditor::highlightCurrentLine()
{
    QTextEdit::ExtraSelection selection;
    auto color = QColor(m_highlighter->theme().editorColor(KSyntaxHighlighting::Theme::CurrentLine));
    color.setAlpha(220);
    selection.format.setBackground(color);
    selection.format.setProperty(QTextFormat::FullWidthSelection, true);
    selection.cursor = textCursor();
    selection.cursor.clearSelection();

    QList<QTextEdit::ExtraSelection> extraSelections;
    extraSelections.append(selection);
    setExtraSelections(extraSelections);
}

QTextBlock CodeEditor::blockAtPosition(int y) const
{
    auto block = firstVisibleBlock();
    if (!block.isValid())
        return QTextBlock();

    int top = blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + blockBoundingRect(block).height();
    do {
        if (top <= y && y <= bottom)
            return block;
        block = block.next();
        top = bottom;
        bottom = top + blockBoundingRect(block).height();
    } while (block.isValid());
    return QTextBlock();
}

bool CodeEditor::isFoldable(const QTextBlock &block) const
{
    return m_highlighter->startsFoldingRegion(block);
}

bool CodeEditor::isFolded(const QTextBlock &block) const
{
    if (!block.isValid())
        return false;
    const auto nextBlock = block.next();
    if (!nextBlock.isValid())
        return false;
    return !nextBlock.isVisible();
}

void CodeEditor::toggleFold(const QTextBlock &startBlock)
{
    // we also want to fold the last line of the region, therefore the ".next()"
    const auto endBlock = m_highlighter->findFoldingRegionEnd(startBlock).next();

    if (isFolded(startBlock)) {
        // unfold
        auto block = startBlock.next();
        while (block.isValid() && !block.isVisible()) {
            block.setVisible(true);
            block.setLineCount(block.layout()->lineCount());
            block = block.next();
        }

    } else {
        // fold
        auto block = startBlock.next();
        while (block.isValid() && block != endBlock) {
            block.setVisible(false);
            block.setLineCount(0);
            block = block.next();
        }
    }

    // redraw document
    document()->markContentsDirty(startBlock.position(),
                                  endBlock.position() - startBlock.position() + 1);

    // update scrollbars
    Q_EMIT document()->documentLayout()->documentSizeChanged(
        document()->documentLayout()->documentSize());
}

void CodeEditor::paintEvent(QPaintEvent *e)
{
    QPlainTextEdit::paintEvent(e);

    QPainter p(viewport());
    for (auto i = _lines.begin(); i != _lines.end(); ++i) {
//        auto b = document()->findBlockByLineNumber(i.key());
        auto rc = blockBoundingGeometry(i.key());
        rc.moveTop(rc.top() - 2);
        rc.setBottom(rc.top() + 2);
        p.fillRect(rc, _formats.value(i.value()).background());
    }
    viewport()->update();
}

void CodeEditor::setHighlighting(const QString &fileName)
{
    const auto def = m_repository.definitionForFileName(fileName);
    m_highlighter->setDefinition(def);
}

void CodeEditor::append(const QString &code, const BlockType &type, Diff::Segment *segment)
{
    //    if (segment == previousSegment) {

    auto t = textCursor();
    //    t.insertBlock(_formats.value(type));
    //    if (!code.isEmpty())
    //        t.insertText(code);
    //    else
    //        t.insertText(" ");
    //    t.block().setUserData(new SegmentData{segment});

    //    assert(code==t.block().text());

    if (_segments.size())
        t.insertBlock();

    QTextCursor c(t.block());
    c.insertText(code);
    _segments.insert(t.block().blockNumber(), segment);
    t.setBlockFormat(_formats.value(type));
    t.block().setUserData(new SegmentData{segment});
    //    }
    //    append(QStringList() << code, type, segment);
}

void CodeEditor::append(const QStringList &code, const BlockType &type, Diff::Segment *segment)
{
    if (!code.size() && (type == Added || type == Removed)) {
        _lines.insert(textCursor().block(), type == Added ? Removed : Added);
        return;
    }
    for (auto &e : code)
        append(e, type, segment);
}

QPair<int, int> CodeEditor::blockArea(int from, int to)
{
    auto firstBlock = document()->findBlockByLineNumber(from);
    auto secondBlock = document()->findBlockByLineNumber(to);
    qDebug() << from << " to " << to << firstBlock.text() << secondBlock.text();

    int top = qRound(blockBoundingGeometry(firstBlock).translated(contentOffset()).top());
    int bottom;

    if (to == -1)
        bottom = top + 1;
    else
        bottom = qRound(blockBoundingGeometry(secondBlock).translated(contentOffset()).bottom());

    return qMakePair(top, bottom);
}

int CodeEditor::currentLineNumber() const
{
    return textCursor().block().firstLineNumber();
}

void CodeEditor::gotoLineNumber(int lineNumber)
{
    QTextBlock block = document()->findBlockByLineNumber(lineNumber);

    if (block.isValid()) {
        QTextCursor cursor(block);
        setTextCursor(cursor);
    }
}

void CodeEditor::mouseReleaseEvent(QMouseEvent *event)
{
    Q_EMIT blockSelected();
    QPlainTextEdit::mouseReleaseEvent(event);
}

Diff::Segment *CodeEditor::currentSegment() const
{
    return _segments.value(textCursor().block().blockNumber(), nullptr);
}

void CodeEditor::highlightSegment(Diff::Segment *segment)
{
    for (auto i = _segments.begin(); i != _segments.end(); i++) {
        if (i.value()==segment) {
            auto block = document()->findBlockByNumber(i.key());
            QTextCursor cursor(block);
            setTextCursor(cursor);
        }
    }
}

void CodeEditor::clearAll()
{
    _segments.clear();
    _lines.clear();
    clear();
}

#include "codeeditor.moc"
