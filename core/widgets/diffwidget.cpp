#include "codeeditor.h"
#include "diffwidget.h"
#include "../widgets/segmentconnector.h"
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

void DiffWidget::setOldFile(const Git::File &newOldFile)
{
    _oldFile = newOldFile;
}

const Git::File &DiffWidget::newFile() const
{
    return _newFile;
}

void DiffWidget::setNewFile(const Git::File &newNewFile)
{
    _newFile = newNewFile;
}

void DiffWidget::compare()
{
    auto segments = Diff::diff(_oldFile.content(), _newFile.content());

    _oldCodeEditor->clearAll();
    _newCodeEditor->clearAll();

    _oldCodeEditor->setHighlighting(_oldFile.fileName());
    _newCodeEditor->setHighlighting(_newFile.fileName());
    _segmentConnector->setSegments(segments);
    _segmentConnector->update();

    for (auto &s: segments) {
        switch (s->type) {
        case Diff::SegmentType::SameOnBoth:
            _oldCodeEditor->append(s->oldText, CodeEditor::Unchanged, s);
            _newCodeEditor->append(s->newText, CodeEditor::Unchanged, s);
            break;
        case Diff::SegmentType::OnlyOnLeft:
            _oldCodeEditor->append(s->oldText, CodeEditor::Removed, s);
            _newCodeEditor->append(s->newText, CodeEditor::Added, s);
            break;
        case Diff::SegmentType::OnlyOnRight:
            _oldCodeEditor->append(s->oldText, CodeEditor::Removed, s);
            _newCodeEditor->append(s->newText, CodeEditor::Added, s);
            break;
        case Diff::SegmentType::DifferentOnBoth:
            _oldCodeEditor->append(s->oldText, CodeEditor::Edited, s);
            _newCodeEditor->append(s->newText, CodeEditor::Edited, s);
            break;
        }
    }
}

void DiffWidget::showHiddenChars(bool show)
{
    if (show) {
        auto n = _defaultOption;
        n.setFlags(QTextOption::ShowTabsAndSpaces | QTextOption::ShowDocumentTerminator);
        _oldCodeEditor->document()->setDefaultTextOption(n);
        _newCodeEditor->document()->setDefaultTextOption(n);
    } else {
        _oldCodeEditor->document()->setDefaultTextOption(_defaultOption);
        _newCodeEditor->document()->setDefaultTextOption(_defaultOption);
    }
}

CodeEditor *DiffWidget::oldCodeEditor() const
{
    return _oldCodeEditor;
}

CodeEditor *DiffWidget::newCodeEditor() const
{
    return _newCodeEditor;
}

DiffWidget::DiffWidget(QWidget *parent) : WidgetBase(parent), _oldFile(), _newFile()
{
    setupUi();
}

DiffWidget::DiffWidget(const Git::File &oldFile, const Git::File &newFile, QWidget *parent)
    : WidgetBase(parent), _oldFile(oldFile), _newFile(newFile)
{
    setupUi();
}

void DiffWidget::setupUi()
{
    _oldCodeEditor = new CodeEditor(this);
    _newCodeEditor = new CodeEditor(this);
    _segmentConnector = new SegmentConnector(this);

    _oldCodeEditor->setReadOnly(true);
    _newCodeEditor->setReadOnly(true);

    auto splitter = new QSplitter(this);
//    layout->setMargin(0);
//    layout->setSpacing(0);

    splitter->addWidget(_oldCodeEditor);
    splitter->addWidget(_segmentConnector);
    splitter->addWidget(_newCodeEditor);
    _segmentConnector->setMinimumWidth(80);
    _segmentConnector->setMaximumWidth(80);
    _segmentConnector->setLeft(_oldCodeEditor);
    _segmentConnector->setRight(_newCodeEditor);

    auto layout = new QHBoxLayout(this);
    layout->addWidget(splitter);
    setLayout(layout);

    _defaultOption = _oldCodeEditor->document()->defaultTextOption();


    connect(_oldCodeEditor,
            &CodeEditor::blockSelected,
            this,
            &DiffWidget::oldCodeEditor_blockSelected);

    connect(_newCodeEditor,
            &CodeEditor::blockSelected,
            this,
            &DiffWidget::newCodeEditor_blockSelected);

    connect(_oldCodeEditor->verticalScrollBar(),
            &QScrollBar::valueChanged,
            this,
            &DiffWidget::oldCodeEditor_scroll);

    connect(_newCodeEditor->verticalScrollBar(),
            &QScrollBar::valueChanged,
            this,
            &DiffWidget::newCodeEditor_scroll);

}


void DiffWidget::oldCodeEditor_scroll(int value)
{
    static bool b{false};
    if (b)
        return;
    b = true;
    _newCodeEditor->verticalScrollBar()->setValue(
        (int) (((float) value / (float) _newCodeEditor->verticalScrollBar()->maximum())
               * (float) _newCodeEditor->verticalScrollBar()->maximum()));
    b = false;
    _segmentConnector->update();
}

void DiffWidget::newCodeEditor_scroll(int value)
{
    static bool b{false};
    if (b)
        return;
    b = true;
    _oldCodeEditor->verticalScrollBar()->setValue(
        (int) (((float) value / (float) _oldCodeEditor->verticalScrollBar()->maximum())
               * (float) _oldCodeEditor->verticalScrollBar()->maximum()));
    b = false;
    _segmentConnector->update();
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
