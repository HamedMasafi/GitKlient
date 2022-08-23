#include "codeeditor.h"
#include "codeeditorsidebar.h"
#include "GitKlientSettings.h"

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
    SegmentData(Diff::Segment *segment, bool empty = false) : _segment{segment}, isEmpty(empty) {}
    Diff::Segment *segment() const;
    void setSegment(Diff::Segment *newSegment);
    bool isEmpty{false};
};

Diff::Segment *SegmentData::segment() const
{
    return _segment;
}

void SegmentData::setSegment(Diff::Segment *newSegment)
{
    _segment = newSegment;
}

CodeEditor::CodeEditor(QWidget *parent)
    : QPlainTextEdit(parent)
      , m_highlighter(new KSyntaxHighlighting::SyntaxHighlighter(document()))
      , m_sideBar(new CodeEditorSidebar(this))
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

    QTextBlockFormat normalFormat, addedFormat, removedFormat, changedFormat, highlightFormat,
        emptyFormat;

    addedFormat.setBackground(GitKlientSettings::diffAddedColor());
    removedFormat.setBackground(GitKlientSettings::diffRemovedColor());
    changedFormat.setBackground(GitKlientSettings::diffModifiedColor());
    highlightFormat.setBackground(Qt::yellow);

    emptyFormat.setBackground(QBrush(Qt::gray, Qt::BDiagPattern));
    //    normalFormat.setBackground(Qt::lightGray);

    _formats.insert(Added, addedFormat);
    _formats.insert(Removed, removedFormat);
    _formats.insert(Unchanged, normalFormat);
    _formats.insert(Edited, changedFormat);
    _formats.insert(HighLight, highlightFormat);
    _formats.insert(Empty, emptyFormat);

    setLineWrapMode(QPlainTextEdit::NoWrap);
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
#if QT_VERSION >= QT_VERSION_CHECK(5, 11, 0)
    return 4 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits + fontMetrics().lineSpacing();
#else
    return 4 + fontMetrics().width(QLatin1Char('9')) * digits + fontMetrics().lineSpacing();
#endif
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
    int lineNumber{0};

//    auto &emptyFormat = _formats.value(Empty);
    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {

            QBrush bg;
            if (blockNumber >= _currentSegment.first && blockNumber <= _currentSegment.second)
                bg = Qt::yellow;
            else
                bg = document()->findBlockByNumber(blockNumber).blockFormat().background();
            painter.fillRect(QRect{0, top, m_sideBar->width() - 1, fontMetrics().height()}, bg);

            painter.setPen(m_highlighter->theme().editorColor(
                (blockNumber == currentBlockNumber) ? KSyntaxHighlighting::Theme::CurrentLineNumber
                                                    : KSyntaxHighlighting::Theme::LineNumbers));

//            if (block.blockFormat() != emptyFormat)
            {
                ++lineNumber;
                const auto number = QString::number(blockNumber + 1);
                painter.drawText(0,
                                 top,
                                 m_sideBar->width() - 2 - foldingMarkerSize,
                                 fontMetrics().height(),
                                 Qt::AlignRight,
                                 number);
            }
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
    setViewportMargins(this->sidebarWidth(), 0, 0, 0);
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

    auto top = blockBoundingGeometry(block).translated(contentOffset()).top();
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

//    QPainter p(viewport());
//    for (auto i = _lines.begin(); i != _lines.end(); ++i) {
////        auto b = document()->findBlockByLineNumber(i.key());
//        auto rc = blockBoundingGeometry(i.key());
//        rc.moveTop(rc.top() - 2);
//        rc.setBottom(rc.top() + 2);
//        p.fillRect(rc, _formats.value(i.value()).background());
//    }
    viewport()->update();
}

void CodeEditor::setHighlighting(const QString &fileName)
{
    const auto def = m_repository.definitionForFileName(fileName);
    m_highlighter->setDefinition(def);
}

void CodeEditor::append(const QString &code, const BlockType &type, Diff::Segment *segment, bool isEmpty)
{
    auto t = textCursor();

    if (_segments.size())
        t.insertBlock();

    QTextCursor c(t.block());
    c.insertText(code);
    _segments.insert(t.block().blockNumber(), segment);
    t.setBlockFormat(_formats.value(type));
    t.block().setUserData(new SegmentData{segment, isEmpty});
}

int CodeEditor::append(const QString &code, const QColor &backgroundColor)
{
    auto t = textCursor();

    if (_segments.size())
        t.insertBlock();

    QTextCursor c(t.block());
    c.insertText(code);
    QTextBlockFormat fmt;
    fmt.setBackground(backgroundColor);
    t.setBlockFormat(fmt);
    _segments.insert(t.block().blockNumber(), nullptr);
    return t.block().blockNumber();
}

void CodeEditor::append(const QStringList &code, const BlockType &type, Diff::Segment *segment, int size)
{
//    if (!code.size() && (type == Added || type == Removed)) {
//        _lines.insert(textCursor().block(), type == Added ? Removed : Added);
//        return;
//    }
    for (auto &e : code)
        append(e, type, segment);
    if (size > code.size())
        for (int var = 0; var < size - code.size(); ++var) {
            append(QString(), Empty, segment, true);
        }
}

QPair<int, int> CodeEditor::blockArea(int from, int to)
{
    auto firstBlock = document()->findBlockByLineNumber(from);
    auto secondBlock = document()->findBlockByLineNumber(to);
//    qDebug() << from << " to " << to << firstBlock.text() << secondBlock.text();

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

void CodeEditor::gotoSegment(Diff::Segment *segment)
{
    for (auto i = _segments.begin(); i != _segments.end(); i++) {
        if (i.value() == segment) {
            QTextBlock block = document()->findBlockByLineNumber(i.key());

            if (block.isValid()) {
                QTextCursor cursor(block);
                setTextCursor(cursor);
            }
            return;
        }
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
    _currentSegment = qMakePair(-1, -1);
    for (auto i = _segments.begin(); i != _segments.end(); i++) {
        if (i.value() == segment) {
            if (_currentSegment.first == -1)
                _currentSegment.first = i.key();
            //            auto block = document()->findBlockByNumber(i.key());

            //            QTextCursor cursor(block);
            ////            cursor.setBlockFormat(_formats.value(HighLight));
            //            setTextCursor(cursor);
            //            return;
        } else if (_currentSegment.first != -1){
            _currentSegment.second = i.key() - 1;
            break;
        }
    }
//    _currentSegment = segment;
    m_sideBar->update();
    qDebug() << _currentSegment;
    return;
    qDebug() << "Segment not found";
}

void CodeEditor::clearAll()
{
    _segments.clear();
    qDeleteAll(_segments);
    _lines.clear();
    clear();
}

