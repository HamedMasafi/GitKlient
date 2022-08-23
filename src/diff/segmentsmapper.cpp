#include "segmentsmapper.h"

#include <QScrollBar>
#include "widgets/codeeditor.h"



SegmentsMapper::SegmentsMapper(QObject *parent) : QObject(parent)
{

}

void SegmentsMapper::addEditor(CodeEditor *editor)
{
    _editors.append(editor);

    connect(editor,
            &CodeEditor::blockSelected,
            this,
            &SegmentsMapper::codeEditor_blockSelected);

    _scrollBars.insert(editor->verticalScrollBar(), editor);
    connect(editor->verticalScrollBar(),
            &QScrollBar::valueChanged,
            this,
            &SegmentsMapper::codeEditor_scroll);
}

const QList<Diff::Segment *> &SegmentsMapper::segments() const
{
    return _segments;
}

void SegmentsMapper::setSegments(const QList<Diff::MergeSegment *> &newSegments)
{
    for (auto &s: newSegments)
        _segments.append(s);
}

int SegmentsMapper::map(int from, int to, int index) const
{
    int offset1{0};
    int offset2{0};
    int offset3{0};
    int &offsetFrom = from == 1 ? offset1 : (from == 2 ? offset2 : offset3);
    int &offsetTo = to == 1 ? offset1 : (to == 2 ? offset2 : offset3);

    for (auto &s : _segments) {
        auto ms = dynamic_cast<Diff::MergeSegment*>(s);

        if (offsetFrom + s->get(from).size() > index) {
            if (s->type != Diff::SegmentType::DifferentOnBoth)
                return offsetFrom + (index - offsetTo);
            else
                return offsetFrom;
        }

        offset1 += ms->base.size();
        offset2 += ms->local.size();
        offset3 += ms->remote.size();
    }
    return -1;
}

void SegmentsMapper::codeEditor_blockSelected()
{
    auto s = qobject_cast<CodeEditor *>(sender());

    _currentSegment = s->currentSegment();
    s->highlightSegment(_currentSegment);

    for (auto &editor : _editors) {
        editor->highlightSegment(_currentSegment);
        editor->gotoSegment(_currentSegment);
        /*if (s == editor)
            continue;
        auto n = map(myIndx, _editors.indexOf(editor), l);

        if (n != -1)
            editor->gotoLineNumber(n);

        editor->highlightSegment(s->currentSegment());*/
    }
}

void SegmentsMapper::codeEditor_scroll(int value)
{
    static QAtomicInt n = 0;
    if (n)
        return;
    n.ref();
    auto s = _scrollBars.value(sender());
    if (!s)
        return;
    for (auto &editor: _editors) {
        if (s == editor)
            continue;
        editor->verticalScrollBar()->setValue(
            (int) (((float) value / (float) s->verticalScrollBar()->maximum())
                   * (float) s->verticalScrollBar()->maximum()));
    }
    n.deref();
}

Diff::Segment *SegmentsMapper::currentSegment() const
{
    return _currentSegment;
}

void SegmentsMapper::refresh()
{
    if (!_currentSegment)
        return;
    for (auto &editor : _editors) {
        editor->highlightSegment(_currentSegment);
        editor->gotoSegment(_currentSegment);
    }
}

void SegmentsMapper::setCurrentSegment(Diff::Segment *newCurrentSegment)
{
    _currentSegment = newCurrentSegment;
    refresh();
}

bool SegmentsMapper::isMergeable() const
{
    for (auto &s : _segments) {
        auto ms = dynamic_cast<Diff::MergeSegment*>(s);
        if (ms->mergeType == Diff::MergeType::None)
            return false;
    }
    return true;
}

int SegmentsMapper::conflicts() const
{
    int r{0};
    for (auto &s: _segments) {
        auto ms = dynamic_cast<Diff::MergeSegment*>(s);
        if (ms->mergeType == Diff::None)
            r++;
    }
    return r;
}

void SegmentsMapper::findPrevious(const Diff::SegmentType &type)
{
    int index;
    if (_currentSegment)
        index = _segments.indexOf(dynamic_cast<Diff::MergeSegment *>(_currentSegment)) - 1;
    else
        index = _segments.size() - 1;

    for (auto i = index; i; i--)
        if (_segments.at(i)->type == type) {
            setCurrentSegment(_segments.at(i));
            return;
        }
}

void SegmentsMapper::findNext(const Diff::SegmentType &type)
{
    int index;
    if (_currentSegment)
        index = _segments.indexOf(dynamic_cast<Diff::MergeSegment *>(_currentSegment)) + 1;
    else
        index = 0;

    for (auto i = index; i < _segments.size(); i++)
        if (_segments.at(i)->type == type) {
            setCurrentSegment(_segments.at(i));
            return;
        }

}
