#ifndef DIFF_H
#define DIFF_H

#include <QtCore/QStringList>

namespace Diff {
int matchesCount(const QStringList &base, const QStringList &local, const QStringList &remote);
QStringList take(QStringList &list, int count);
int remove(QStringList &list, int count);

#define DiffTypeEnum(x) enum class x { Unchanged, Added, Removed, Modified };

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
    virtual ~Segment() = default;

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
    ~DiffSegment() override = default;

    QStringList get(int index) override
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
    ~MergeSegment() override = default;
    QStringList base;
    QStringList local;
    QStringList remote;
//    MergeDiffType type;
    MergeType mergeType{None};

    QStringList get(int index) override
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


QList<MergeSegment *> diff3(const QString &base,
                            const QString &local,
                            const QString &remote);
QList<MergeSegment *> diff3(const QStringList &base,
                            const QStringList &local,
                            const QStringList &remote);

QMap<QString, DiffType> diffDirs(const QString &dir1, const QString &dir2);
} // namespace Diff

#endif // DIFF_H
