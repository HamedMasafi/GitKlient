#include "segmentsmapper.h"

SegmentsMapper::SegmentsMapper() {}

const QList<Diff::MergeSegment *> &SegmentsMapper::segments() const
{
    return _segments;
}

void SegmentsMapper::setSegments(const QList<Diff::MergeSegment *> &newSegments)
{
    _segments = newSegments;
}

int SegmentsMapper::mapIndexFromOldToNew(int oldIndex)
{
    auto offset = qMakePair(0, 0);
    for (auto &s : _segments) {
        if (offset.first + s->local.size() > oldIndex) {
            if (s->type != Diff::SegmentType::DifferentOnBoth)
                return offset.second + (oldIndex - offset.first);
            else
                return offset.second;
        }
        offset.first += s->local.size();
        offset.second += s->remote.size();
    }
    return -1;
}

int SegmentsMapper::mapIndexFromNewToOld(int newIndex)
{
    auto offset = qMakePair(0, 0);
    for (auto &s : _segments) {
        if (offset.second + s->remote.size() > newIndex) {
            if (s->type != Diff::SegmentType::DifferentOnBoth)
                return offset.first + (newIndex - offset.second);
            else
                return offset.first;
        }
        offset.first += s->local.size();
        offset.second += s->remote.size();
    }
    return -1;
}

int SegmentsMapper::map(int from, int to, int index) const
{
    int offset1{0};
    int offset2{0};
    int offset3{0};
    int &offsetFrom = from == 1 ? offset1 : (from == 2 ? offset2 : offset3);
    int &offsetTo = to == 1 ? offset1 : (to == 2 ? offset2 : offset3);

    for (auto &s : _segments) {
        if (offsetFrom + s->get(from).size() > index) {
            if (s->type != Diff::SegmentType::DifferentOnBoth)
                return offsetFrom + (index - offsetTo);
            else
                return offsetFrom;
        }

        offset1 += s->base.size();
        offset2 += s->local.size();
        offset3 += s->remote.size();
    }
    return -1;
}
