#include "gitloglist.h"
#include "gitlog.h"
#include "gitmanager.h"
#include "gitgraphlane.h"

#include <QDateTime>
#include <QDebug>

namespace Git {

struct LanesData
{
    QStringList hashes;
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

        if (index==connection)
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
        for (int i = 1; i < childHashes.size(); ++i) {
            auto n = set(-1, childHashes.at(i), GraphLane::Start, index);
//            qDebug() << "Inserted" << n;
        }
    }
    QList<int> find(const QString &hash)
    {
        QList<int> ret;
        for (int i = 0; i < hashes.size(); ++i) {
            if (hashes.at(i)==hash)
                ret.append(i);
        }
        return ret;
    }
    void merge(const QString &commitHash, const QStringList &childHashes) {
        auto list = find(commitHash);
        int firstIndex{-1};

        for (auto &index : list) {
            if (childHashes.size() && firstIndex == -1) {
                firstIndex = index;
                set(index, childHashes.first(), GraphLane::Node);
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
        if (!log->parentHash().size()) {
            init(log->childs());
        } else if (log->parentHash().size() > 1) {
            merge(log->commitHash(), log->childs());
        } else if (log->childs().size() > 1) {
            fork(log->commitHash(), log->childs());
        } else if (log->childs().size()) {
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
        for (auto &p: log->parentHash())
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
        if (!log->parentHash().isEmpty()) {
            auto parent = findByHash(log->parentHash().first());
            if (parent->refLog() != log->refLog() && log->parentHash().size() == 1) {
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
    }
}

}
