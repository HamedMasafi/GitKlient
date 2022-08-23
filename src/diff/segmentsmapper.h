#ifndef SEGMENTSMAPPER_H
#define SEGMENTSMAPPER_H

#include "diff/diff.h"
#include <QMap>
#include <QObject>

class CodeEditor;
class QScrollBar;
class SegmentsMapper : public QObject
{
    Q_OBJECT

public:
    explicit SegmentsMapper(QObject *parent = nullptr);

    void addEditor(CodeEditor *editor);
    const QList<Diff::Segment *> &segments() const;
    void setSegments(const QList<Diff::MergeSegment *> &newSegments);

    int map(int from, int to, int index) const;

    Diff::Segment *currentSegment() const;
    void refresh();

    void setCurrentSegment(Diff::Segment *newCurrentSegment);
    bool isMergeable() const;

    int conflicts() const;

    void findPrevious(const Diff::SegmentType &type);
    void findNext(const Diff::SegmentType &type);

private slots:
    void codeEditor_blockSelected();
    void codeEditor_scroll(int value);

private:
    Diff::Segment *_currentSegment{nullptr};
    QList<Diff::Segment *> _segments;
    QList<CodeEditor *> _editors;
    QMap<QObject *, CodeEditor *> _scrollBars;
};

#endif // SEGMENTSMAPPER_H
