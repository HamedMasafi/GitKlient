#ifndef DIFF_H
#define DIFF_H

#include <QtCore/QStringList>

namespace Diff {
int matchesCount(const QStringList &base, const QStringList &local, const QStringList &remote);
QStringList take(QStringList &list, int count);
int remove(QStringList &list, int count);

enum class DiffType

{
    Unchanged,
    Added,
    Removed,
    Modified
};

enum MergeType
{
    None,
    KeepLocal,
    KeepRemote,
    KeepLocalThenRemote,
    KeepRemoteThenLocal,
    MergeCustom
};

enum class SegmentType
{
    SameOnBoth,
    OnlyOnLeft,
    OnlyOnRight,
    DifferentOnBoth,
};

enum MergeDiffType
{
    Unchanged,
    LocalAdd,
    RemoteAdd,
    BothChanged
/*
    Unchanged = SegmentType::SameOnBoth,
    LocalAdd = SegmentType::OnlyOnLeft,
    RemoteAdd = SegmentType::OnlyOnRight,
    BothChanged = SegmentType::DifferentOnBoth
*/
};

struct Segment {
    virtual ~Segment() {}

    QStringList oldText;
    QStringList newText;
    SegmentType type;

    virtual QStringList get(int index)
    {
        switch (index) {
        case 0:
            return oldText;
        case 1:
            return newText;
        }
        return QStringList();
    }
};

struct DiffSegment : Segment
{
    virtual ~DiffSegment() {}

    QStringList get(int index)
    {
        switch (index) {
        case 0:
            return oldText;
        case 1:
            return newText;
        }
        return QStringList();
    }
};

struct MergeSegment : Segment
{
    virtual ~MergeSegment() {}
    QStringList base;
    QStringList local;
    QStringList remote;
//    MergeDiffType type;
    MergeType mergeType{None};

    QStringList get(int index)
    {
        switch (index) {
        case 0:
            return base;
        case 1:
            return local;
        case 2:
            return remote;
        }
        return QStringList();
    }
    MergeSegment();
    MergeSegment(const QStringList &base, const QStringList &local, const QStringList &remote);
};

QList<Segment *> diff(const QString &oldText, const QString &newText);
QList<Segment *> diff(const QStringList &oldText, const QStringList &newText);
QList<MergeSegment *> diff3_2(const QStringList &base,
                            const QStringList &local,
                            const QStringList &remote);
QList<MergeSegment *> diff3(const QStringList &base,
                            const QStringList &local,
                            const QStringList &remote);

QString gitDiff(QList<Segment*> segments);

QPair<int, int> firstMatch(const QStringList &list1, const QStringList &list2);
}; // namespace Diff

#endif // DIFF_H
