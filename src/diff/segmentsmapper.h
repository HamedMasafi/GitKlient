#ifndef SEGMENTSMAPPER_H
#define SEGMENTSMAPPER_H

#include "diff/diff.h"

class SegmentsMapper
{
    QList<Diff::MergeSegment *> _segments;

public:
    SegmentsMapper();

    const QList<Diff::MergeSegment *> &segments() const;
    void setSegments(const QList<Diff::MergeSegment *> &newSegments);

    int mapIndexFromOldToNew(int oldIndex);
    int mapIndexFromNewToOld(int newIndex);
    int map(int from, int to, int index) const;
};

#endif // SEGMENTSMAPPER_H
