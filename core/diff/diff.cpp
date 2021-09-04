#include "diff.h"

#include <QDebug>
#include <QDir>
#include <QSet>
#include <QDateTime>
#include <set>

namespace Diff
{

namespace Impl {

struct Pair3 {
    int first;
    int second;
    int third;

    Pair3() : first{-1}, second{-1}, third{-1} {}
    Pair3(int firstNumber, int secondNumber, int thirdNumber)
        : first{firstNumber}, second{secondNumber}, third{thirdNumber}
    {}

    bool operator==(const Pair3 &other)
    {
        return first == other.first && second == other.second && third == other.third;
    }
};
typedef QList<QPair<int, int>> Solution;
typedef QList<Pair3> Solution3;

QSet<Solution> findLCS(QStringList first, QStringList second, int m, int n)
{
    // construct a set to store possible LCS
    int matrix[120][120];
    QSet<Solution> s;

    // If we reaches end of either string, return
    // a empty set
    if (m == 0 || n == 0)
    {
        s.insert(Solution());
        return s;
    }

    // If the last characters of X and Y are same
    if (first[m - 1] == second[n - 1])
    {
        // recurse for X[0..m-2] and Y[0..n-2] in
        // the matrix
        auto tmp = findLCS(first, second, m - 1, n - 1);
        // append current character to all possible LCS
        // of substring X[0..m-2] and Y[0..n-2].
        for (auto &str : qAsConst(tmp)) {
            auto buffer = str;
            buffer.push_back(qMakePair(m - 1, n - 1));
            s.insert(buffer);
        }
    }

    // If the last characters of X and Y are not same
    else
    {
        // If LCS can be constructed from top side of
        // the matrix, recurse for X[0..m-2] and Y[0..n-1]
        if (matrix[m - 1][n] >= matrix[m][n - 1]) {
            auto tmp = findLCS(first, second, m - 1, n);
            s = s + tmp;
        }

        // If LCS can be constructed from left side of
        // the matrix, recurse for X[0..m-1] and Y[0..n-2]
        if (matrix[m][n - 1] >= matrix[m - 1][n])

        {
            auto tmp = findLCS(first, second, m, n - 1);
            // merge two sets if L[m-1][n] == L[m][n-1]
            // Note s will be empty if L[m-1][n] != L[m][n-1]
            //            s.insert(tmp.begin(), tmp.end());
            s = s + tmp;
        }
    }

    return s;
}

int maxIn(int first, int second, int third)
{
    if (first == second && second == third)
        return 0;

    if (first > second && first > third)
        return 1;

    if (second > first && second > third)
        return 2;

    if (third > first && third > second)
        return 3;

    return 4;
}

int maxIn(int first, int second)
{
    if (first == second)
        return 0;

    if (first > second)
        return 1;

    if (second > first)
        return 2;

    return 4;
}
Solution3 longestCommonSubsequence(const QStringList &source,
                                   const QStringList &target,
                                   const QStringList &target2)
{
    // Mostly stolen from https://www.geeksforgeeks.org/printing-longest-common-subsequence/

    QMap<int, QMap<int, QMap<int, int>>> l;
    for (int i = 0; i <= source.count(); i++) {
        for (int j = 0; j <= target.count(); j++) {
            for (int k = 0; k <= target2.count(); ++k) {
                if (i == 0 || j == 0 || k == 0) {
                    l[i][j][k] = 0;
                } else if (source.at(i - 1) == target.at(j - 1) && source.at(i - 1) == target2.at(k - 1) ) {
                    l[i][j][k] = l[i - 1][j - 1][k - 1] + 1;
                } else {
                    l[i][j][k] = std::max(std::max(l[i - 1][j][k], l[i][j - 1][k]), l[i][j][k - 1]);
                }
            }
        }
    }

    int i = source.count();
    int j = target.count();
    int k = target2.count();
    int index = l[source.count()][target.count()][target2.count()];
    QString longestCommonSubsequence;
    Solution3 r;
    while (i > 0 && j > 0 && k > 0) {
        if (source.at(i - 1) == target.at(j - 1) && source.at(i - 1) == target2.at(k - 1)) {
            //            longestCommonSubsequence[index - 1] = source.at(i - 1);
            r.prepend({i - 1, j - 1, k - 1});
            i--;
            j--;
            k--;
            index--;
////        } else if (l[i - 1][j][k] > l[i][j - 1][k] && l[i - 1][j][k] > l[i][j][k - 1]) {
////            i--;
////        } else if (l[i][j][k - 1] > l[i][j - 1][k] && l[i][j][k - 1] > l[i - 1][j][k]) {
////            k--;
        } else {
            int n = maxIn(l[i - 1][j][k], l[i][j - 1][k], l[i][j][k - 1]);
            switch (n) {
            case 1:
                i--;
                break;
            case 2:
                j--;
                break;
            case 3:
                k--;
                break;
            default:
//                r.prepend({i - 1, j - 1, k - 1});
                i--;
                j--;
                k--;
                break;
            }
//            j--;
        }
    }

    return r;
}
Solution longestCommonSubsequence(const QStringList &source, const QStringList &target)
{
    // Mostly stolen from https://www.geeksforgeeks.org/printing-longest-common-subsequence/

    QMap<int, QMap<int, int>> l;
    for (int i = 0; i <= source.count(); i++) {
        for (int j = 0; j <= target.count(); j++) {
            if (i == 0 || j == 0) {
                l[i][j] = 0;
            } else if (source.at(i - 1) == target.at(j - 1)) {
                l[i][j] = l[i - 1][j - 1] + 1;
            } else {
                l[i][j] = std::max(l[i - 1][j], l[i][j - 1]);
            }
        }
    }

    int i = source.count();
    int j = target.count();
    int index = l[source.count()][target.count()];
    QString longestCommonSubsequence;
    Solution r;
    while (i > 0 && j > 0) {
        if (source.at(i - 1) == target.at(j - 1)) {
            //            longestCommonSubsequence[index - 1] = source.at(i - 1);
            r.prepend(qMakePair(i - 1, j - 1));
            i--;
            j--;
            index--;
//        } else if (l[i - 1][j] > l[i][j - 1]) {
//            i--;
//        } else {
//            j--;
//        }
        } else {
            int n = maxIn(l[i - 1][j], l[i][j - 1]);
            switch (n) {
            case 1:
                i--;
                break;
            case 2:
                j--;
                break;
            default:
                //                r.prepend({i - 1, j - 1, k - 1});
                i--;
                j--;
                break;
            }
            //            j--;
        }
    }

    qDebug() << r;
    return r;
}

static bool stringListCompare(Solution a, Solution b)
{
    return (a.size() < b.size());
}
}
struct Tuple {
    int base;
    int local;
    int remote;
};


QDebug &operator<<(QDebug &stream, const Tuple &t)
{
    stream << t.base << t.local << t.remote;
    return stream;
}

Tuple firstCommonItem(const QStringList &base, const QStringList &local, const QStringList &remote)
{
    Tuple tmp{-1, -1, -1};
    for (int i = 0; i < base.size(); ++i) {
        auto localIndex = local.indexOf(base.at(i));
        auto remoteIndex = remote.indexOf(base.at(i));

        if (localIndex !=-1 && remoteIndex!=-1) {
            if (tmp.base == -1) {
                tmp.base = i;
                tmp.local = localIndex;
                tmp.remote = remoteIndex;
                continue;
            }
            auto sum = i + localIndex + remoteIndex;
            auto tmpSum = tmp.base + tmp.local + tmp.remote;
            if (sum < tmpSum) {
                tmp.base = i;
                tmp.local = localIndex;
                tmp.remote = remoteIndex;
                continue;
            }
        }
    }
    return tmp;
}

QPair<int, int> unmatchesCount(const QStringList &base, const QStringList &other)
{
    for (int i = 0; i < base.size(); ++i) {
        for (int j = 0; j < other.size(); ++j) {
            if (base.at(i) == other.at(j))
                return qMakePair(i, j);
        }
    }
    return qMakePair(-1, -1);
}

int matchesCount(const QStringList &base, const QStringList &local)
{
    int ret = 0;
    while (base.size() > ret && local.size() > ret && base.at(ret) == local.at(ret))
        ret++;
    return ret;
}

int matchesCount(const QStringList &base, const QStringList &local, const QStringList &remote)
{
    int ret = 0;
    while (base.size() > ret && local.size() > ret && remote.size() > ret &&
           base.at(ret) == local.at(ret) && base.at(ret) == remote.at(ret) )
        ret++;
    return ret;
}

QStringList take(QStringList &list, int count)
{
    QStringList ret = list.mid(0, qMin(count, list.size()));
    remove(list, count);
    return ret;
}

int remove(QStringList &list, int count)
{
    int size{qMin(count, list.size())};
    for (int i = 0; i < size; ++i)
        list.removeFirst();
    return size;
}

QList<MergeSegment *> diff3(const QStringList &baseList, const QStringList &localList, const QStringList &remoteList)
{
    auto max = Impl::longestCommonSubsequence(baseList, localList, remoteList);

    auto base = baseList;
    auto local = localList;
    auto remote = remoteList;

    int baseOffset{0};
    int localOffset{0};
    int remoteOffset{0};
    QList<MergeSegment *> ret;

    Impl::Pair3 p;
    while (max.size()) {
        if (p == Impl::Pair3())
            p = max.takeFirst();

        if (p.first == baseOffset && p.second == localOffset && p.third == remoteOffset) {
            auto segment = new MergeSegment;
            segment->type = SegmentType::SameOnBoth;
            while (p.first == baseOffset && p.second == localOffset) {
                baseOffset++;
                localOffset++;
                remoteOffset++;
                segment->base.append(base.takeFirst());
                segment->local.append(local.takeFirst());
                segment->remote.append(remote.takeFirst());

                if (max.size())
                    p = max.takeFirst();
            }
            ret.append(segment);
            if (!max.size())
                break;
        } else {
            if (!max.size())
                break;
            p = max.takeFirst();
        }

        QStringList _baseList, _localList, _remoteList;
        if (baseOffset - p.first)
            _baseList = take(base, p.first - baseOffset);
        if (localOffset - p.second)
            _localList = take(local, p.second - localOffset);
        if (remoteOffset - p.third)
            _remoteList = take(remote, p.third - remoteOffset);

        baseOffset += _baseList.size();
        localOffset += _localList.size();
        remoteOffset += _remoteList.size();

        auto segment = new MergeSegment{_baseList, _localList, _remoteList};

        ret.append(segment);
    }

    return ret;
}

QList<MergeSegment *> diff3_2(const QStringList &baseList, const QStringList &localList, const QStringList &remoteList)
{
    QList<MergeSegment *> segments;
    auto base = baseList;
    auto local = localList;
    auto remote = remoteList;
    while (true) {
        if (!base.size()) {
            segments.append(new MergeSegment{base, local, remote});
            break;
        }

        int sameSize = matchesCount(base, local, remote);
        /*      int matchWithLocal = matchesCount(base, local);
        int matchWithRemote = matchesCount(base, remote);

        if (matchWithLocal == 0 && matchWithRemote == 0) {
            auto unmatchWithLocal = unmatchesCount(base, local);
            auto unmatchWithRemote= unmatchesCount(base, remote);

            auto baseSubList = take(base, qMin(unmatchWithLocal.first, unmatchWithRemote.first));
            auto localSubList = take(local, unmatchWithLocal.second);
            auto remoteSubList = take(remote, unmatchWithRemote.second);
            segments.append(new DiffSegment{baseSubList, localSubList, remoteSubList});
            continue;
        }
        auto baseSubList = take(base, qMin(matchWithLocal, matchWithRemote));
        auto localSubList = take(local, matchWithLocal);
        auto remoteSubList = take(remote, matchWithRemote);
        segments.append(new DiffSegment{baseSubList, localSubList, remoteSubList});
        */
            if (sameSize) {
            auto segment = new MergeSegment;
            segment->type = SegmentType::SameOnBoth;// Unchanged;
            segment->base = take(base, sameSize);
            remove(local, sameSize);
            remove(remote, sameSize);
            segments.append(segment);
            continue;
        } else {
            auto common = firstCommonItem(base, local, remote);
            qDebug() << "Common:" << common;

            if (common.base == -1) {
                segments.append(new MergeSegment{base, local, remote});
                break;
            }
            if (common.base == 0) {
                auto localSubList = take(local, common.local);
                auto remoteSubList = take(remote, common.remote);
                segments.append(new MergeSegment{QStringList(), localSubList, remoteSubList});
                continue;
            }
            auto baseSubList = take(base, common.base);
            auto localSubList = take(local, common.local);
            auto remoteSubList = take(remote, common.remote);
            segments.append(new MergeSegment{baseSubList, localSubList, remoteSubList});
            qDebug() << " * BASE:  " << baseSubList;
            qDebug() << " * LOCAL: " << localSubList;
            qDebug() << " * REMOTE:" << remoteSubList;
            continue;
        }
    }
    return segments;
}

QPair<int, int> firstMatch(const QStringList &list1, const QStringList &list2)
{
    QPair<int, int> ret = qMakePair(-1, -1);
    for (int n1 = 0; n1 < list1.size(); ++n1)
        for (int n2 = 0; n2 < list2.size(); ++n2)
            if (list1.at(n1) == list2.at(n2)) {
                if (ret.first == -1) {
                    ret.first = n1;
                    ret.second = n2;
                    continue;
                }
                if (qMin(ret.first, ret.second) > qMin(n1, n2)) {
                    ret.first = n1;
                    ret.second = n2;
                    continue;
                }
            }
    return ret;;
}

MergeSegment::MergeSegment()
{

}

MergeSegment::MergeSegment(const QStringList &base, const QStringList &local, const QStringList &remote)
    : base{base}, local{local}, remote{remote}
{
    if (local.size() && remote.size())
        type = SegmentType::DifferentOnBoth;
    else if (!local.size() && remote.size())
        type = SegmentType::OnlyOnRight;
    else if (local.size() && !remote.size())
        type = SegmentType::OnlyOnLeft;
    else
        type = SegmentType::SameOnBoth;

    oldText = local;
    newText = remote;

    if (type == SegmentType::SameOnBoth)
        this->base = this->local;
}

QList<Segment *> diff(const QStringList &oldText, const QStringList &newText)
{
    if (oldText == newText) {
        auto segment = new Segment;
        segment->type = SegmentType::SameOnBoth;
        segment->oldText = oldText;
        segment->newText = newText;
        return {segment};
    } else if (oldText.isEmpty()) {
        auto segment = new Segment;
        segment->type = SegmentType::OnlyOnLeft;
        segment->oldText = oldText;
        segment->newText = newText;
        return {segment};
    } else if (newText.isEmpty()) {
        auto segment = new Segment;
        segment->type = SegmentType::OnlyOnRight;
        segment->oldText = oldText;
        segment->newText = newText;
        return {segment};
    }

    auto o = oldText;
    auto n = newText;
    auto max = Impl::longestCommonSubsequence(oldText, newText);
    int oldOffset{0};
    int newOffset{0};
    QList<Segment *> ret;

    QPair<int, int> p;
    while (max.size()) {
        if (p == QPair<int, int>())
            p = max.takeFirst();

        if (p.first == oldOffset && p.second == newOffset) {
            auto segment = new Segment;
            segment->type = SegmentType::SameOnBoth;
            while (p.first == oldOffset && p.second == newOffset) {
                oldOffset++;
                newOffset++;
                segment->oldText.append(o.takeFirst());
                segment->newText.append(n.takeFirst());

                if (max.size())
                    p = max.takeFirst();
            }
            ret.append(segment);
            if (!max.size())
                break;
        } else {
            if (!max.size())
                break;
            p = max.takeFirst();
        }

        QStringList oldList, newList;
        if (oldOffset - p.first)
            oldList = take(o, p.first - oldOffset);
        if (newOffset - p.second)
            newList = take(n, p.second - newOffset);

        oldOffset += oldList.size();
        newOffset += newList.size();

        auto segment = new Segment;
        segment->oldText = oldList;
        segment->newText = newList;

        if (oldList.size() &&newList.size())
            segment->type = SegmentType::DifferentOnBoth;
        else if (oldList.size())
            segment->type = SegmentType::OnlyOnLeft;
        else
            segment->type = SegmentType::OnlyOnRight;
        ret.append(segment);
    }

    return ret;
}

QList<Segment *> diff(const QString &oldText, const QString &newText)
{
    QStringList oldList, newList;
    if (!oldText.isEmpty())
        oldList = oldText.split("\n");
    if (!newText.isEmpty())
        newList = newText.split("\n");

    return diff(oldList, newList);
}

QString gitDiff(QList<Segment *> segments)
{
    QString s;

    return s;
}

void browseDir(QStringList &filesList, const QString &dirPath, const QString &basePath)
{
    QDir d(basePath + "/" + dirPath);
    auto files = d.entryList(QDir::NoDotAndDotDot | QDir::Files);
    for (auto &f : files) {
        if (dirPath.isEmpty())
            filesList.append(dirPath + f);
        else
            filesList.append(dirPath + "/" + f);
    }

    auto dirs = d.entryList(QDir::NoDotAndDotDot | QDir::Dirs);
    for (auto &d : dirs)
        browseDir(filesList, dirPath + d, basePath);
}

bool isFilesSame(const QString &file1, const QString &file2)
{
    QFileInfo fi1{file1};
    QFileInfo fi2{file2};

    if (fi1.size() != fi2.size() || fi1.lastModified() != fi2.lastModified())
        return false;

    QFile f1{file1};
    QFile f2{file2};

    if (!f1.open(QIODevice::ReadOnly) || !f2.open(QIODevice::ReadOnly))
        return false;

    while (!f1.atEnd() && !f2.atEnd()) {
        auto buffer1 = f1.read(100);
        auto buffer2 = f2.read(100);
        if (buffer1 != buffer2)
            return false;
    }

    f1.close();
    f2.close();

    return true;
}

QMap<QString, DiffType> diffDirs(const QString &dir1, const QString &dir2)
{
    auto d1 = dir1;
    auto d2 = dir2;
    QMap<QString, DiffType> map;

    if (!d1.endsWith("/"))
        d1.append("/");

    if (!d2.endsWith("/"))
        d2.append("/");

    QStringList files1, files2;
    browseDir(files1, "", d1);
    browseDir(files2, "", d2);

    for (auto &file: files1) {
        if (!files2.contains(file)) {
            map.insert(file, DiffType::Removed);
        } else {
            map.insert(file,
                       isFilesSame(d1 + file, d2 + file) ? DiffType::Unchanged : DiffType::Modified);
            files2.removeOne(file);
        }
    }

    for (auto &file: files2)
        map.insert(file, DiffType::Added);

    return map;
}

}
