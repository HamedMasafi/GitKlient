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
    Diff::Segment *_currentSegment{nullptr};

public:
    SegmentsMapper(QObject *parent = nullptr);

    void addEditor(CodeEditor *editor);
    const QList<Diff::MergeSegment *> &segments() const;
    void setSegments(const QList<Diff::MergeSegment *> &newSegments);

    int mapIndexFromOldToNew(int oldIndex);
    int mapIndexFromNewToOld(int newIndex);
    int map(int from, int to, int index) const;

    Diff::Segment *currentSegment() const;
    void refresh();

    void setCurrentSegment(Diff::Segment *newCurrentSegment);
    bool isMergeable() const;

    int conflicts() const;

private slots:
    void codeEditor_blockSelected();
    void codeEditor_scroll(int value);

private:
    QList<Diff::MergeSegment *> _segments;
    QList<CodeEditor *> _editors;
    QMap<QObject *, CodeEditor *> _scrollBars;
};

#endif // SEGMENTSMAPPER_H
