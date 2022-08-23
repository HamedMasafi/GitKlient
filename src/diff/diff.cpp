#include "diff.h"

#include <QDebug>
#include <QDir>
#include <QDateTime>
#include <set>
#include <QElapsedTimer>

namespace Diff
{

namespace Impl {

typedef QPair<int, int> Pair2;
struct Pair3 {
    int first;
    int second;
    int third;

    Pair3() : first{-1}, second{-1}, third{-1} {}
    Pair3(int firstNumber, int secondNumber, int thirdNumber)
        : first{firstNumber}, second{secondNumber}, third{thirdNumber}
    {}

    bool operator==(const Pair3 &other) const
    {
        return first == other.first && second == other.second && third == other.third;
    }
};

QDebug operator<<(QDebug d, const Pair3 &p)
{
    d.noquote() << "(" << p.first << "," << p.second << "," << p.third << ")";
    return d;
}

typedef QList<Pair2> Solution;
typedef QList<Pair3> Solution3;
/*
QSet<Solution> findLCS(QStringList first, QStringList second, int m, int n)
{
    // construct a set to store possible LCS
    int matrix[120, 120];
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
        if (matrix[m - 1, n] >= matrix[m, n - 1]) {
            auto tmp = findLCS(first, second, m - 1, n);
            s = s + tmp;
        }

        // If LCS can be constructed from left side of
        // the matrix, recurse for X[0..m-1] and Y[0..n-2]
        if (matrix[m, n - 1] >= matrix[m - 1, n])

        {
            auto tmp = findLCS(first, second, m, n - 1);
            // merge two sets if L[m-1, n] == L[m, n-1]
            // Note s will be empty if L[m-1, n] != L[m, n-1]
            //            s.insert(tmp.begin(), tmp.end());
            s = s + tmp;
        }
    }

    return s;
}
*/
int maxIn(int first, int second, int third)
{
    if (first == second && second == third)
        return 0;

    if (third > first && third > second)
        return 3;

    if (first > second && first > third)
        return 1;

    if (second > first && second > third)
        return 2;


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

int maxIn(const QList<int> &list) {
    if (list.empty())
        return -1;
    int max{list.first()};
    int maxIndex{0};
    int index{0};
    for (const auto &i: list) {
        if (i  >  max)  {
            max = i;
            maxIndex = index;
        }
        index++;
    }

    return maxIndex;
}

template <typename T>
class Array2
{
    T *_data;
    int c1, c2;

public:
    Array2(int c1, int c2);
    ~Array2();

    Q_ALWAYS_INLINE T &operator()(int i1, int i2);

    void exportToCsv(const QString &file);
};


template<typename T>
Q_OUTOFLINE_TEMPLATE Array2<T>::Array2(int c1, int c2) : c1(c1), c2(c2)
{
    _data = new T[c1 * c2];
}

template<typename T>
Q_OUTOFLINE_TEMPLATE Array2<T>::~Array2()
{
    delete[] _data;
}

template<typename T>
Q_OUTOFLINE_TEMPLATE T &Array2<T>::operator()(int i1, int i2)
{
    return _data[c1 * i2 + i1];
}

template<typename T>
Q_OUTOFLINE_TEMPLATE void Array2<T>::exportToCsv(const QString &file)
{
    QFile f(file);
    f.open(QIODevice::WriteOnly | QIODevice::Text);
    for (int i = 0; i <= c1; i++) {
        for (int j = 0; j <= c2; j++)
                f.write(QByteArray::number(operator()(i, j)) + ",");
        f.write("\n");
    }
    f.close();
}
template <typename T>
class Array3
{
    T *_data;
    int c1, c2, c3;

public:
    Array3(int c1, int c2, int c3);
    ~Array3();

    Q_ALWAYS_INLINE T &operator()(int i1, int i2, int i3);
};

template<typename T>
Q_OUTOFLINE_TEMPLATE Array3<T>::Array3(int c1, int c2, int c3) : c1(c1), c2(c2), c3(c3)
{
    _data = new T[c1 * c2 * c3];
}

template<typename T>
Q_OUTOFLINE_TEMPLATE Array3<T>::~Array3()
{
    delete[] _data;
}

template<typename T>
Q_OUTOFLINE_TEMPLATE T &Array3<T>::operator()(int i1, int i2, int i3)
{
    return _data[i3 + (c2 * c1 * i1) + (c3 * i2)];
}

Solution3 longestCommonSubsequence(const QStringList &source,
                                   const QStringList &target,
                                   const QStringList &target2)
{
    Array3<int> l(source.size() + 1, target.size() + 1, target2.size() + 1);

    for (int i = 0; i <= source.count(); i++)
        for (int j = 0; j <= target.count(); j++)
            for (int k = 0; k <= target2.count(); ++k)
                if (i == 0 || j == 0 || k == 0) {
                    l(i, j, k) = 0;
                } else if (source.at(i - 1) == target.at(j - 1)
                           && source.at(i - 1) == target2.at(k - 1)) {
                    l(i, j, k) = l(i - 1, j - 1, k - 1) + 1;
                } else {
                    l(i, j, k) = std::max(std::max(l(i - 1, j, k), l(i, j - 1, k)), l(i, j, k - 1));
                }

    int i = source.count();
    int j = target.count();
    int k = target2.count();
    int index = l(source.count(), target.count(), target2.count());

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
            int n = maxIn({l(i - 1, j, k),
                           l(i, j - 1, k),
                           l(i, j, k - 1),

                           l(i - 1, j - 1, k),
                           l(i, j - 1, k - 1),
                           l(i - 1, j, k - 1),

                           l(i - 1, j - 1, k - 1)});
            switch (n) {
            case 0:
                i--;
                break;
            case 1:
                j--;
                break;
            case 2:
                k--;
                break;

            case 3:
                i--;
                j--;
                break;
            case 4:
                j--;
                k--;
                break;
            case 5:
                i--;
                k--;
                break;

            case 6:
                r.prepend({i - 1, j - 1, k - 1});
                i--;
                j--;
                k--;
                break;
            default:
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

template<typename T>
inline bool isEqual(const T &i1, const T &i2) {
    return i1 == i2;
}
template<>
inline bool isEqual<QString>(const QString &i1, const QString &i2) {
    return i1.trimmed() == i2.trimmed();
}

Solution longestCommonSubsequence(const QStringList &source, const QStringList &target)
{
    Array2<int> l(source.size() + 1, target.size() + 1);

    for (int i = 0; i <= source.count(); i++) {
        for (int j = 0; j <= target.count(); j++) {
            if (i == 0 || j == 0) {
                l(i, j) = 0;
            } else if (source.at(i - 1) == target.at(j - 1)) {
                l(i, j) = l(i - 1, j - 1) + 1;
            } else {
                l(i, j) = qMax(l(i - 1, j), l(i, j - 1));
            }
        }
    }

    int i = source.count();
    int j = target.count();
    int index = l(source.count(), target.count());
    Solution r;

    while (i > 0 && j > 0) {
        if (isEqual(source.at(i - 1), target.at(j - 1))) {
            r.prepend(qMakePair(i - 1, j - 1));
            i--;
            j--;
            index--;
        } else {
            int n = maxIn(l(i - 1, j), l(i, j - 1), l(i - 1, j - 1));
            switch (n) {
            case 1:
                i--;
                break;
            case 2:
                j--;
                break;
            default:
                i--;
                j--;
                break;
            }
        }
    }

    return r;
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

QStringList readFileLines(const QString &filePath)
{
    //    QStringList buffer;
    QFile f{filePath};
    if (!f.open(QIODevice::ReadOnly))
        return QStringList();

    //    while (!f.atEnd()) {
    //        buffer << f.readLine();
    //    }
    auto buf = QString(f.readAll()).split('\n');
    f.close();
    return buf;
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
    while (!max.empty()) {
        if (p == Impl::Pair3())
            p = max.takeFirst();

        if (p.first == baseOffset && p.second == localOffset && p.third == remoteOffset) {
            auto segment = new MergeSegment;
            segment->type = SegmentType::SameOnBoth;
            while (p.first == baseOffset && p.second == localOffset && p.third == remoteOffset) {
                baseOffset++;
                localOffset++;
                remoteOffset++;
                segment->base.append(base.takeFirst());
                segment->local.append(local.takeFirst());
                segment->remote.append(remote.takeFirst());

                if (!max.empty())
                    p = max.takeFirst();
            }
            ret.append(segment);
//            if (!max.size())
//                break;
        } else {
//            if (!max.size())
//                break;
//            p = max.takeFirst();
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

    if (base.empty() && local.empty() && remote.empty()) {
        auto segment = new MergeSegment{base, local, remote};
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
        if (base.empty()) {
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
            continue;
        }
    }
    return segments;
}

    MergeSegment::MergeSegment() = default;

MergeSegment::MergeSegment(const QStringList &base, const QStringList &local, const QStringList &remote)
    : base{base}, local{local}, remote{remote}
{
    if (local == remote)
        type = SegmentType::SameOnBoth;
    else if (base.empty() && !local.empty())
        type = SegmentType::OnlyOnLeft;
    else if (base.empty() && !remote.empty())
        type = SegmentType::OnlyOnRight;
    else
        type = SegmentType::DifferentOnBoth;

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
    while (!max.empty()) {
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

                if (!max.empty())
                    p = max.takeFirst();
            }
            ret.append(segment);
            if (max.empty())
                break;
        } else {
            if (max.empty())
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

        if (!oldList.empty() && !newList.empty())
            segment->type = SegmentType::DifferentOnBoth;
        else if (!oldList.empty())
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
    for (auto &d : dirs) {
        if (dirPath.isEmpty())
            browseDir(filesList, dirPath + d, basePath);
        else
            browseDir(filesList, dirPath + "/" + d, basePath);
    }
}

bool isFilesSame(const QString &file1, const QString &file2)
{
    QFileInfo fi1{file1};
    QFileInfo fi2{file2};

    if (fi1.size() != fi2.size()/* || fi1.lastModified() != fi2.lastModified()*/)
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

QList<MergeSegment *> diff3(const QString &base, const QString &local, const QString &remote)
{
    QStringList baseList, localList, remoteList;
    if (!base.isEmpty())
        baseList = base.split("\n");
    if (!local.isEmpty())
        localList = local.split("\n");
    if (!remote.isEmpty())
        remoteList = remote.split("\n");

    return diff3(baseList, localList, remoteList);
}

}
