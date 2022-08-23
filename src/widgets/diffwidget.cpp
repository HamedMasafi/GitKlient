#include "codeeditor.h"
#include "diffwidget.h"
#include "diff/diff.h"

#include <QHBoxLayout>
#include <QScrollBar>
#include <QTextBlock>
#include <QDebug>
#include <QSplitter>

const Git::File &DiffWidget::oldFile() const
{
    return _oldFile;
}

void DiffWidget::setOldFileText(const Git::File &newOldFile)
{
    lineEditOldFileName->setText(newOldFile.displayName());
}

void DiffWidget::setOldFile(const Git::File &newOldFile)
{
    _oldFile = newOldFile;
    setOldFileText(newOldFile);
}

const Git::File &DiffWidget::newFile() const
{
    return _newFile;
}

void DiffWidget::setNewFileText(const Git::File &newNewFile)
{
    lineEditNewFileName->setText(newNewFile.displayName());
}

void DiffWidget::setNewFile(const Git::File &newNewFile)
{
    _newFile = newNewFile;
    setNewFileText(newNewFile);
}

void DiffWidget::compare()
{
    auto segments = Diff::diff(_oldFile.content(), _newFile.content());

    leftCodeEditor->clearAll();
    rightCodeEditor->clearAll();

    leftCodeEditor->setHighlighting(_oldFile.fileName());
    rightCodeEditor->setHighlighting(_newFile.fileName());
    segmentConnector->setSegments(segments);
    segmentConnector->update();

    for (auto &s: segments) {
        CodeEditor::BlockType oldBlockType, newBlockType;
        switch (s->type) {
        case Diff::SegmentType::SameOnBoth:
            oldBlockType = newBlockType = CodeEditor::Unchanged;
            break;
        case Diff::SegmentType::OnlyOnLeft:
            oldBlockType = CodeEditor::Removed;
            newBlockType = CodeEditor::Added;
            break;
        case Diff::SegmentType::OnlyOnRight:
            oldBlockType = CodeEditor::Removed;
            newBlockType = CodeEditor::Added;
            break;
        case Diff::SegmentType::DifferentOnBoth:
            oldBlockType = newBlockType = CodeEditor::Edited;
            break;
        }

        if (m_sameSize) {
            int size = qMax(s->oldText.size(), s->newText.size());
            leftCodeEditor->append(s->oldText, oldBlockType, s, size);
            rightCodeEditor->append(s->newText, newBlockType, s, size);
        } else {
            leftCodeEditor->append(s->oldText, oldBlockType, s);
            rightCodeEditor->append(s->newText, newBlockType, s);
        }
    }

    scrollToTop();
}

void DiffWidget::showHiddenChars(bool show)
{
    if (show) {
        auto n = _defaultOption;
        n.setFlags(QTextOption::ShowTabsAndSpaces | QTextOption::ShowDocumentTerminator);
        leftCodeEditor->document()->setDefaultTextOption(n);
        rightCodeEditor->document()->setDefaultTextOption(n);
    } else {
        leftCodeEditor->document()->setDefaultTextOption(_defaultOption);
        rightCodeEditor->document()->setDefaultTextOption(_defaultOption);
    }
    leftCodeEditor->setWordWrapMode(QTextOption::NoWrap);
    rightCodeEditor->setWordWrapMode(QTextOption::NoWrap);
}

void DiffWidget::showFilesInfo(bool show)
{
    filesInfoWidget->setVisible(show);
}

void DiffWidget::showSameSize(bool show)
{
    m_sameSize = show;
    segmentConnector->setSameSize(show);
    compare();
}

void DiffWidget::on_splitter_splitterMoved(int, int)
{
    recalculateInfoPaneSize();
}

CodeEditor *DiffWidget::oldCodeEditor() const
{
    return leftCodeEditor;
}

CodeEditor *DiffWidget::newCodeEditor() const
{
    return rightCodeEditor;
}

DiffWidget::DiffWidget(QWidget *parent) : WidgetBase(parent), _oldFile(), _newFile()
{
    setupUi(this);
    segmentConnector->setMinimumWidth(80);
    segmentConnector->setMaximumWidth(80);
    segmentConnector->setLeft(leftCodeEditor);
    segmentConnector->setRight(rightCodeEditor);

    connect(leftCodeEditor,
            &CodeEditor::blockSelected,
            this,
            &DiffWidget::oldCodeEditor_blockSelected);

    connect(rightCodeEditor,
            &CodeEditor::blockSelected,
            this,
            &DiffWidget::newCodeEditor_blockSelected);

    connect(leftCodeEditor->verticalScrollBar(),
            &QScrollBar::valueChanged,
            this,
            &DiffWidget::oldCodeEditor_scroll);

    connect(rightCodeEditor->verticalScrollBar(),
            &QScrollBar::valueChanged,
            this,
            &DiffWidget::newCodeEditor_scroll);

    recalculateInfoPaneSize();

    _defaultOption = leftCodeEditor->document()->defaultTextOption();
}

DiffWidget::DiffWidget(const Git::File &oldFile, const Git::File &newFile, QWidget *parent)
    : WidgetBase(parent), _oldFile(oldFile), _newFile(newFile)
{
    setupUi(this);
    segmentConnector->setMinimumWidth(80);
    segmentConnector->setMaximumWidth(80);
    segmentConnector->setLeft(leftCodeEditor);
    segmentConnector->setRight(rightCodeEditor);

    connect(leftCodeEditor,
            &CodeEditor::blockSelected,
            this,
            &DiffWidget::oldCodeEditor_blockSelected);

    connect(rightCodeEditor,
            &CodeEditor::blockSelected,
            this,
            &DiffWidget::newCodeEditor_blockSelected);

    connect(leftCodeEditor->verticalScrollBar(),
            &QScrollBar::valueChanged,
            this,
            &DiffWidget::oldCodeEditor_scroll);

    connect(rightCodeEditor->verticalScrollBar(),
            &QScrollBar::valueChanged,
            this,
            &DiffWidget::newCodeEditor_scroll);

    recalculateInfoPaneSize();

    _defaultOption = leftCodeEditor->document()->defaultTextOption();
}

void DiffWidget::oldCodeEditor_scroll(int value)
{
    static bool b{false};
    if (b)
        return;
    b = true;
    rightCodeEditor->verticalScrollBar()->setValue(
        (int) (((float) value / (float) rightCodeEditor->verticalScrollBar()->maximum())
               * (float) rightCodeEditor->verticalScrollBar()->maximum()));
    b = false;
    segmentConnector->update();
}

void DiffWidget::newCodeEditor_scroll(int value)
{
    static bool b{false};
    if (b)
        return;
    b = true;
    leftCodeEditor->verticalScrollBar()->setValue(
        (int) (((float) value / (float) leftCodeEditor->verticalScrollBar()->maximum())
               * (float) leftCodeEditor->verticalScrollBar()->maximum()));
    b = false;
    segmentConnector->update();
}

void DiffWidget::oldCodeEditor_blockSelected()
{
    //    auto b = _oldCodeEditor->textCursor().block().blockNumber();
//    auto b = _oldCodeEditor->currentSegment();
//    if (b) {
//        _segmentConnector->setCurrentSegment(b);
//        _newCodeEditor->highlightSegment(b);
//    }
}

void DiffWidget::newCodeEditor_blockSelected()
{
//    auto b = _newCodeEditor->currentSegment();
//    if (b) {
//        _segmentConnector->setCurrentSegment(b);
//        _oldCodeEditor->highlightSegment(b);
    //    }
}

void DiffWidget::recalculateInfoPaneSize()
{
    leftInfoContainer->setMinimumWidth(leftCodeEditor->width());
    rightInfoContainer->setMinimumWidth(rightCodeEditor->width());

    leftInfoContainer->setVisible(leftCodeEditor->width());
    rightInfoContainer->setVisible(rightCodeEditor->width());

//    label->setMinimumWidth(leftCodeEditor->width());
//    label_2->setMinimumWidth(rightCodeEditor->width());
}

void DiffWidget::resizeEvent(QResizeEvent *event)
{
    WidgetBase::resizeEvent(event);
    recalculateInfoPaneSize();
}

void DiffWidget::showEvent(QShowEvent *event)
{
    Q_UNUSED(event)
    recalculateInfoPaneSize();
}

bool DiffWidget::sameSize() const
{
    return m_sameSize;
}

void DiffWidget::setSameSize(bool newSameSize)
{
    if (m_sameSize == newSameSize)
        return;
    m_sameSize = newSameSize;
    emit sameSizeChanged();
}

void DiffWidget::scrollToTop()
{
    leftCodeEditor->setTextCursor(QTextCursor(leftCodeEditor->document()->findBlockByNumber(0)));
    rightCodeEditor->setTextCursor(QTextCursor(rightCodeEditor->document()->findBlockByNumber(0)));
//    leftCodeEditor->verticalScrollBar()->setValue(0);
//    rightCodeEditor->verticalScrollBar()->setValue(0);
    segmentConnector->update();
}
