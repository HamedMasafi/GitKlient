#include "gitloglist.h"
#include "gitlog.h"
#include "gitmanager.h"
#include "gitgraphlane.h"

#include <QDateTime>
#include <QDebug>

namespace Git {

struct LanesFactory2 {
    QStringList hashes;

    QList<int> findHashes(const QString &hash) {
        int index{0};
        QList<int> ret;
        for (auto const &h: qAsConst(hashes)) {
            if (hash == h)
                ret.append(index);
            index++;
        }
        return ret;
    }
    QList<int> setHashes(const QStringList &hashesList) {
        QList<int> ret;
        QMutableListIterator<QString> i(hashes);
        int index{0};

        while (i.hasNext() && index < hashesList.size()) {
            auto hash = i.next();
            if (hash == QString()) {
                i.setValue(hashesList.at(index));
                ret.append(index);
                index++;
            }
        }
        for (int j = 0; j < hashesList.size() - index; ++j) {
            hashes.append(hashesList.at(index));
            ret.append(index);
            index++;
        }
        return ret;
    }
    void set(const int &index, const GraphLane &lane, QVector<GraphLane> &lanes) {
        if (index < lanes.size())
            lanes.replace(index, lane);
        else
            lanes.append(lane);

    }
    QVector<GraphLane> apply(Log *log)
    {
        QVector<GraphLane> lanes;
        GraphLane *firstLane{nullptr};
        lanes.reserve(hashes.size());
        int firstIndex{-1};

        for (int i = 0; i < hashes.size(); i++) {
            if (hashes.at(i) != QString())
                set(i, GraphLane::Pipe, lanes);
        }

        auto list = findHashes(log->commitHash());
        for (auto i = list.begin(); i != list.end(); ++i) {
            if (firstIndex == -1) {
                firstIndex = *i;
                set(*i, GraphLane::Node, lanes);
            } else {
                GraphLane lane{GraphLane::End};
                lane._bottomJoins = {firstIndex};
                set(*i, lane, lanes);
            }
            hashes.replace(*i, QString());
        }

        list = setHashes(log->childs());
        auto childs = log->childs();
        lanes.reserve(hashes.size());
        firstIndex = -1;
        for (auto i = list.begin(); i != list.end(); ++i) {
            if (firstIndex == -1) {
                firstIndex = *i;
                set(*i, GraphLane::Node, lanes);
            } else {
                GraphLane lane{GraphLane::Start};
                lane._upJoins = {firstIndex};
                set(*i, lane, lanes);
            }
            hashes.replace(*i, childs.takeFirst());
        }
        return lanes;
    }
};
struct LanesFactory
{
private:
    struct Data2 {
        QString parentHash;
        QString childHash;
        GraphLane lane;
    };

    QStringList childs;
    QVector<GraphLane> lanes;

    void normalizeLanes()
    {
        for (auto &lane: lanes) {
            if (lane.type() == GraphLane::Start)
                lane.setType(GraphLane::Pipe);

            if (lane.type() == GraphLane::End)
                lane.setType(GraphLane::None);
        }
    }
    int add(const QString &childHash)
    {
        if (!childs.size()) {
            childs.append(childHash);
            lanes.append(GraphLane::Start);
            return 0;
        }

        for (int i = 0; i < childs.size(); ++i) {
            if (childs.at(i) == QString()) {
                childs.replace(i, childHash);
                lanes.replace(i, GraphLane::Start);
                return i;
            }
        }

        childs.append(childHash);
        lanes.append(GraphLane::Start);
        return childs.size() - 1;
    }

public:
    QVector<GraphLane> apply(Log *log)
    {
        Q_ASSERT(childs.size() == lanes.size());
        //        Q_ASSERT(childs.count(QString()) == lanes.count(GraphLane()));
        normalizeLanes();

        for (int i = 0; i < childs.size(); ++i) {
            auto hash = childs[i];
            if (hash == log->commitHash()) {
                lanes.replace(i, GraphLane::End);
                childs.replace(i, QString());
            }
        }
        int firstIndex{-1};
        for (auto &child : log->childs()) {
            auto i = add(child);
            if (firstIndex == -1) {
                firstIndex = i;
            } else {
                lanes[i]._upJoins.append(firstIndex);
            }
        }
        return lanes;
    }

//    QStringList parents;
//    QStringList childs;

//    QVector<GraphLane> lanes;
    struct Data {
        QString parent;
        QString child;
        bool isNull;
    };

    int findEmpty() {
        for (int i =0;i<lanes.size();++i)
            if (lanes[i] == GraphLane())
                return i;
        lanes.append(GraphLane());
        childs.append(QString());
        return lanes.size() - 1;
    }
    QList<int> findByParent(const QString &hash)
    {
        QList<int> ret;
        int idx{0};
        for (auto &p: childs) {
            if (p == hash)
                ret << idx;
            idx++;
        }
        return ret;
    }

    QVector<GraphLane> apply2(Log *log)
    {
        for (int i = 0; i < lanes.size(); ++i) {
            auto lane = lanes[i];
            if (lane.type() == GraphLane::End)
                lane = GraphLane{};
            else if (lane.type() == GraphLane::Node)
                lane = GraphLane(GraphLane::Pipe);
            else if (lane.type() == GraphLane::Start)
                lane = GraphLane(GraphLane::Pipe);

            lane._joinFrom = lane._joinTo = -1;
            lanes.replace(i, lane);
        }

        auto myList = findByParent(log->commitHash());

        int firstIndex{-1};
        for (auto &p: myList) {
            if (firstIndex!=-1)
                firstIndex = p;
            else {
                lanes[p]._joinTo = firstIndex;
                lanes[p]._type = GraphLane::End;
            }
        }

        firstIndex = -1;
        for (auto &ch: log->childs()) {
            auto fi = findEmpty();
            lanes[fi]._type = GraphLane::Start;
            childs[fi] = ch;

            if (firstIndex!=-1)
                firstIndex = fi;
            else {
                lanes[fi]._joinFrom = firstIndex;
            }
        }

        return lanes;
    }
};
struct LanesData2
{
    QStringList hashes;
    QStringList parents;
    QStringList childs;
    int activeCol;
    QVector<GraphLane> lanes;
    QStringList branchNames;

    int set(int index, const QString &hash, const GraphLane::Type &type, int connection)
    {
        GraphLane lane;

        if (index >= 0 && index < hashes.size()) {
            lane = lanes[index];
            if ((lane.type() == GraphLane::Start && type == GraphLane::End)
                || (lane.type() == GraphLane::End && type == GraphLane::Start))
                lane._type = GraphLane::Node;
            else
                lane._type = type;
        } else {
            lane = GraphLane{type};
        }

        if (type == GraphLane::Start)
            lane._joinTo = connection;
        else if (type == GraphLane::End) {
            lane._joinFrom = connection;
//            if (lane.type() == GitGraphLane::Pipe)
//                lane._type = GitGraphLane::End;
        }

        if (index == connection)
            qDebug() << "777777777777777";

//        if (index != connection)
//        lane._type = GitGraphLane::Transparent;
        return set(index, hash, std::move(lane));
    }

    int set(int index, const QString &hash, const GraphLane &lane)
    {
//        auto lane = GitGraphLane{type};
        if (index == -1)
            index = hashes.indexOf(QString());
        if (index == -1)
            index = hashes.size();

        if (index==-1)
            qDebug() << "*************************";
        if (hashes.size() <= index)
            hashes.append(hash);
        else
            hashes.replace(index, hash);

        if (lanes.size() <= index)
            lanes.append(lane);
        else
            lanes.replace(index, lane);
        return index;
    }

    void init(const QStringList &childHashes)
    {
        for (auto &child: childHashes) {
            set(-1, child, GraphLane::Start);
        }
    }
    void fork(const QString &commitHash, const QStringList &childHashes)
    {
        auto index = hashes.indexOf(commitHash);
        index = set(index, childHashes.first(), GraphLane::Node);
//        qDebug() << "Inserted" << commitHash << index;
        for (auto &hash: childHashes) {
            set(-1, hash, GraphLane::Start, index);
        }
    }
    QList<int> find(const QString &hash)
    {
        QList<int> ret;
        for (int i = 0; i < hashes.size(); ++i) {
            if (hashes.at(i) == hash)
                ret.append(i);
        }
        return ret;
    }
    void merge(const QString &commitHash) {
        auto list = find(commitHash);
        int firstIndex{-1};

        for (auto &index : list) {
            if (firstIndex == -1) {
                firstIndex = index;
                set(index, commitHash, GraphLane::Node);
            } else {
                set(index, QString(), GraphLane::End, firstIndex);
            }
        }
    }
    void updateHash(const QString &commitHash, const QString &childHash)
    {
        auto index = hashes.indexOf(commitHash);
        if (index!=-1)
            set(index, childHash, GraphLane::Node);
    }

/*
    int insert(const QString &commitHash, const QStringList &childHashes)
    {
        auto index = hashes.indexOf(commitHash);
        if (index == -1) {
            qDebug() << "[ERR] 6";
            index = 0;
        }
        if (!childHashes.size())
            return -1;
        set(index, childHashes.at(0), GitGraphLane{GitGraphLane::Node});

        for (int i = 0; i < childHashes.size() - 1; ++i) {
            set(-1, childHashes.at(0), GitGraphLane{GitGraphLane::Start});
        }
        return 0;
    }

    int insert(const QString &hash)
    {
        Q_ASSERT(hashes.size() == lanes.size());
        int index;
//        auto index = hashes.indexOf(hash);
//        if (index != -1)
//            return index;

        index = hashes.indexOf(QString());

        if (index == -1) {
            hashes.append(hash);
            lanes.append(GitGraphLane{GitGraphLane::Start});
            return hashes.size() - 1;
        } else {
            hashes.replace(index, hash);
            lanes.replace(index, GitGraphLane{GitGraphLane::Node});
            return index;
        }
    }

    int fork(int count, const QString &parentHash, const QString &hash)
    {
        Q_ASSERT(hashes.size() == lanes.size());

        auto index = hashes.indexOf(parentHash);
        if (index==-1)
            qDebug() << "[ERR] 1";

        updateHash(parentHash, hash);
        for (int i = 1; i < count; ++i) {
            auto n = insert(hash);
            lanes[n]._joinCol = index;
            lanes[n]._type = GitGraphLane::Start;
        }
        lanes[index]._type = GitGraphLane::Node;
        return 0;
    }
    int merge(const QStringList &parentHash, const QString &commitHash)
    {
        int idx{0};
        for (int i = 0; i < parentHash.size(); ++i) {
            auto &p = parentHash.at(i);
            auto index = hashes.indexOf(p);

            if (index == -1)
                qDebug() << "[ERR] 2" << commitHash ;
            else {
                if (i == 0) {
                    hashes.replace(index, commitHash);
                    lanes.replace(index, GitGraphLane{GitGraphLane::Node});
                    idx = index;
                } else {
                    hashes.replace(index, QString());
                    lanes.replace(index, GitGraphLane{GitGraphLane::End, idx});
                }
            }
        }
        return 0;
    }
    int updateHash(const QString &parentHash, const QString &commitHash)
    {
        auto index = hashes.indexOf(parentHash);
        if (index==-1) {
            qDebug() << "[ERR] 3" << parentHash;
            return -1;
        }
        lanes.replace(index, GitGraphLane{GitGraphLane::Node});
        hashes.replace(index, commitHash);
        return index;
    }
*/
    QVector<GraphLane> apply(Log *log)
    {
        /*QMutableVectorIterator<GraphLane> it(lanes);
        while (it.hasNext()) {
            auto &item = it.next();
            switch (item.type()) {
            case GraphLane::End:
                it.setValue(GraphLane());
            }
        }*/

        if (!log->parents().size()) {
            init(log->childs());
        } else if (log->parents().size() == 1) {
        } else if (log->parents().size() > 1) {
            merge(log->commitHash());
        }

        if (log->childs().size() > 1) {
            fork(log->commitHash(), log->childs());
        }
        else if (log->childs().size() == 1) {
            updateHash(log->commitHash(), log->childs().first());
        } else if (!log->childs().size()) {
            for (int i = 0; i < lanes.size(); ++i) {
                auto lane = lanes[i];
                if (lane.type() != GraphLane::None && lane.type() != GraphLane::Transparent)
                    lanes.replace(i, GraphLane::End);
            }
        }

        auto list = lanes;
        for (int i = 0; i < lanes.size(); ++i) {
            auto lane = lanes[i];
            if (lane.type() == GraphLane::End)
                lane = GraphLane{};
            else if (lane.type() == GraphLane::Node)
                lane = GraphLane(GraphLane::Pipe);
            else if (lane.type() == GraphLane::Start)
                lane = GraphLane(GraphLane::Pipe);

            lane._joinFrom = lane._joinTo = -1;
            lanes.replace(i, lane);
        }

        {
#if QT_VERSION < QT_VERSION_CHECK(5, 14, 0)
            auto set = hashes.toSet();
#else
            QSet<QString> set{hashes.begin(), hashes.end()};
#endif

            if (hashes.size() != set.size()) {
//                qDebug() << hashes.size() << set.size();
            }
//            Q_ASSERT(hashes.size() == set.size());
        }
        return list;
    }
};
struct LanesData
{
    QVector<GraphLane> lanes;
    QStringList hashes;

    void clear(const QList<int> &list) {
        for (const auto &l: list) {
            hashes.replace(l, QString());
            lanes.replace(l, GraphLane::Test);
        }
    }
    int findOfCreate(const QString &hash) {
        auto n = hashes.indexOf(hash);

        if (n != -1)
            return n;

        n = hashes.indexOf(QString());
        if (n != -1) {
            hashes.replace(n, hash);
            lanes.replace(n, GraphLane::Test);
            return n;
        }

        hashes.append(hash);
        lanes.append(GraphLane::Pipe);
        n = hashes.size() - 1;

        return n;
    }
    QList<int> findAll(const QStringList &list) {
        QList<int> ret;
        for (const auto &h: list)
            ret.append(findOfCreate(h));
        return ret;
    }
    QList<int> findAll(const QString &hash) {
        QList<int> ret;
        int n = 0;
        while (true) {
            n = hashes.indexOf(hash, n + 1);
            if (n == -1)
                break;
            ret.append(n);
        }
        return ret;
    }
    void clear() {
        int index = 0;
        for (const auto &lane: qAsConst(lanes)) {
            if (lane.type() == GraphLane::End) {
                hashes.replace(index, QString());
                lanes.replace(index, GraphLane());
            }
            if (lane.type() == GraphLane::Node) {
                lanes.replace(index, GraphLane::Pipe);
            }
            index++;
        }
    }
    QVector<GraphLane> apply(Log *log) {
        Q_ASSERT(hashes.size() == lanes.size());

        auto myHashList = findAll(log->commitHash());
        if (myHashList.size() == 1) {
            hashes.replace(myHashList.first(), log->commitHash());
            lanes[myHashList.first()]._type = GraphLane::Node;
        } else if (myHashList.size() > 1) {
            hashes.replace(myHashList.first(), log->commitHash());
            lanes[myHashList.first()]._bottomJoins = myHashList.mid(1);
            clear(myHashList.mid(1));
        }
        auto childrenList = findAll(log->childs());
        auto newLanes = lanes;
        if (childrenList.size() >= 1) {
            GraphLane ll{GraphLane::Start};
            auto fn = childrenList.first();
            auto otherChildrenList = childrenList.mid(1);

            if (childrenList.size() > 1) {
                for (const auto &o: qAsConst(otherChildrenList)) {
                    ll._upJoins.append(o);
                }
            }
            newLanes.replace(fn, ll);
        }
        clear();
        return newLanes;
    }
};

void readLine(const QString &line, const QString &seprator, QList<QString *> list)
{
    auto parts = line.split(seprator);
    if (parts.size() != list.size())
        return;

    for (auto i = 0; i < parts.size(); i++)
        *list[i] = parts.at(i);
}

QString LogList::branchName(const QString &refLog)
{
    if (refLog.isEmpty())
        return QString();

//    auto parts = refLog.split(",");
//    if (parts.size() < 2)
//        return QString();

//    return parts.at(1).mid(1, parts.at(1).indexOf(")") - 1);
for (auto &b: _branches)
    if (refLog.contains(b))
        return b;
return QString();
}

void LogList::initChilds()
{
    for (auto i = rbegin(); i != rend(); i++) {
        auto &log = *i;
        for (auto &p: log->parents())
            _dataByCommitHashLong.value(p)->_childs.append(log->commitHash());
    }
}

const QString &LogList::branch() const
{
    return _branch;
}

void LogList::setBranch(const QString &newBranch)
{
    _branch = newBranch;
}

Log *LogList::findByHash(const QString &hash, int *index) const
{
    for (int i = 0; i < size(); ++i) {
        if (at(i)->commitHash() == hash) {
            if (index)
                *index = i;
            return at(i);
        }
    }
    if (index)
        *index = -1;
    return nullptr;
}

LogList::LogList() : QList<Log *>() {}

LogList::LogList(const QString &branch) : QList<Log *>(), _branch(branch) {}

void LogList::load()
{
    /*
% -- raw %                    N -- notes                    d -- ref name in brackets     n -- newline
B -- raw body                 P -- parent hash              e -- encoding                 p -- abbreviated parent hash
C -- color                    T -- tree hash                f -- sanitized subject        s -- subject
D -- ref name                 a -- author details           g -- reflog                   t -- abbreviated tree hash
G -- GPG details              b -- body                     h -- abbreviated commit hash  w -- switch line wrapping
H -- commit hash              c -- committer details        m -- mark                     x -- hex code
*/
    qDeleteAll(*this);
    clear();
    _dataByCommitHashLong.clear();

    _branches = Manager::instance()->branches();

    QStringList args{"--no-pager",
                     "log",
                     "--topo-order",
                     "--no-color",
                     "--parents",
                     "--boundary",
                     "--pretty=format:'SEP%m%HX%hX%P%n"
                     "%cnX%ceX%cI%n"
                     "%anX%aeX%aI%n"
                     "%d%n"
                     "%at%n"
                     "%s%n"
                     "%b%n'"};

    if (_branch.size())
        args.insert(2, _branch);

    auto ret = QString(Manager::instance()->runGit(args));
    if (ret.startsWith("fatal:"))
        return;

    auto parts = ret.split("SEP>");

    for (auto &p : parts) {
        auto lines = p.split("\n");
        if (lines.size() < 4)
            continue;

        auto d = new Log;
        QString commitDate;
        QString authDate;
        QString parentHash;
        readLine(lines.at(0), "X", {&d->_commitHash, &d->_commitShortHash, &parentHash});
        readLine(lines.at(1), "X", {&d->_committerName, &d->_committerEmail, &commitDate});
        readLine(lines.at(2), "X", {&d->_authorName, &d->_authorEmail, &authDate});

        if (!parentHash.isEmpty())
            d->_parentHash = parentHash.split(" ");
        d->_refLog = lines.at(3);
        d->_subject = lines.at(5);
        d->_commitDate = QDateTime::fromString(commitDate, Qt::ISODate);
        d->_authDate = QDateTime::fromString(authDate, Qt::ISODate);
        d->_body = lines.mid(5).join("\n");
        append(d);
        _dataByCommitHashLong.insert(d->commitHash(), d);
        _dataByCommitHashLong.insert(d->commitShortHash(), d);
    }
//    std::sort(begin(), end(), [](GitLog *log1,GitLog *log2){
//        return log1->commitDate() < log2->commitDate();
//    });
    initChilds();
    initGraph();
}

void LogList::initGraph()
{
    LanesData2 factory;
    for (auto i = rbegin(); i != rend(); i++) {
        auto &log = *i;
        log->_lanes = factory.apply(log);
    }

    /*
    LanesFactory2 factory;
    for (auto i = rbegin(); i != rend(); i++) {
        auto &log = *i;
        log->_lanes = factory.apply(log);
    }
    return;
/*
    LanesData cols;

    for (auto i = rbegin(); i != rend(); i++) {
        auto &log = *i;
//        bool isFork = log->_childs.size() > 1;
//        bool isMerge = log->parentHash().size() > 1;
//        bool isInitial = log->parentHash().isEmpty();

        auto bn = branchName(log->refLog());
        if (!bn.isEmpty()) {
            log->_branch = bn;
        }
        if (!log->parents().isEmpty()) {
            auto parent = findByHash(log->parents().first());
            if (parent->refLog() != log->refLog() && log->parents().size() == 1) {
//                isFork = true;
                log->_extraData="fork";
            }
        }

        if (log->subject() == "merge back") {
            qDebug() << "Break";
        }
//        if (isInitial) {
////            qDebug() << log->subject() << "is initial";
//            log->_extraData = "initial";
//            cols.insert(log->commitHash());
//        } else if (isFork) {
////            qDebug() << log->subject() << "is fork";
//            log->_extraData = "fork";
//            cols.fork(log->_childs.size(), log->parentHash().at(0), log->commitHash());
//        } else if (isMerge) {
////            qDebug() << log->subject() << "is merge";
//            log->_extraData = "merge";
//            cols.merge(log->parentHash(), log->commitHash());
//        } else {
////            qDebug() << log->subject() << "is normal";
//            log->_extraData = "normal";
//            cols.updateHash(log->parentHash().at(0), log->commitHash());
//        }
//        cols.insert(log->commitHash(), log->childs());

        log->_lanes = cols.apply(log);
        log->_extraData=cols.hashes.join(", ");
    }*/
}

}
