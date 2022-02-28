#include "gitloglist.h"
#include "gitlog.h"
#include "gitmanager.h"
#include "gitgraphlane.h"

#include <QDateTime>
#include <QDebug>

namespace Git {

struct LanesFactory {

    QList<int> findHashes(const QString &hash, const QStringList &hashes) {
        int index{0};
        QList<int> ret;
        for (auto const &h: qAsConst(hashes)) {
            if (hash == h)
                ret.append(index);
            index++;
        }
        return ret;
    }

    QVector<GraphLane> initLanes(const QStringList &hashes, const QString &myHash, int &myIndex)
    {
        int index{0};
        QVector<GraphLane> lanes;
        lanes.reserve(hashes.size());
        for (const auto &hash : hashes) {
            if (hash == QString()) {
                lanes.append(GraphLane::Transparent);
            } else {
                if (hash == myHash) {
                    lanes.append(GraphLane::Node);
                    myIndex = index;
                } else {
                    lanes.append(hash == myHash ? GraphLane::Node : GraphLane::Pipe);
                }
            }
            index++;
        }
        return lanes;
    }

    QList<int> setHashes(const QStringList &hashesList, QStringList &hashes, const int &myIndex) {
        QList<int> ret;
        bool myIndexSet{myIndex == -1 ? true : false};
        int index;

        for (const auto &h: hashesList) {

            if (!myIndexSet) {
                index = myIndex;
                myIndexSet = true;
            } else {
                index = hashes.indexOf(h);
            }
            if (index == -1)
                index = hashes.indexOf(QString());

            if (index == -1) {
                hashes.append(h);
                index = hashes.size() - 1;
            } else {
                hashes.replace(index, h);
            }
            ret.append(index);
        }
        return ret;
    }


    void join(const QString &hash, QStringList &hashList, QVector<GraphLane> &lanes, const int &myIndex) {
        int firstIndex{-10};
        auto list = findHashes(hash, hashList);
        for (auto i = list.begin(); i != list.end(); ++i) {
            if (firstIndex == -1) {
                firstIndex = *i;
                set(*i, GraphLane::Node, lanes);
            } else {
                if (*i==myIndex) {
                                    set(*i, GraphLane::Node, lanes);
                } else {
                    GraphLane lane{GraphLane::Transparent};
                    lane._bottomJoins = {myIndex};
                    set(*i, lane, lanes);
                }
            }
            hashes.replace(*i, QString());
        }
    }

    void fork(const QStringList &childrenList, QStringList &hashList, QVector<GraphLane> &lanes, bool isStart, const int &myInedx) {
        auto list = setHashes(childrenList, hashList, myInedx);
        auto children = childrenList;
        int firstIndex{myInedx};
        lanes.reserve(hashes.size());
//        firstIndex = -1;
        for (auto i = list.begin(); i != list.end(); ++i) {
            if (firstIndex == -1) {
                firstIndex = myInedx;// *i;
                set(*i, isStart ? GraphLane::Start : GraphLane::Node, lanes);
            } else {
                if (*i==myInedx){
                    set(*i, isStart ? GraphLane::Start : GraphLane::Node, lanes);
                } else {
                    GraphLane lane{GraphLane::Transparent};
                    lane._upJoins = {myInedx};
                    set(*i, lane, lanes);
                }
            }
            hashes.replace(*i, children.takeFirst());
        }
    }

    QStringList hashes;

    /*QList<int> findHashes(const QString &hash) {
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
    */
    void set(const int &index, const GraphLane &lane, QVector<GraphLane> &lanes) {
        if (index < lanes.size())
            lanes.replace(index, lane);
        else
            lanes.append(lane);

    }
    QVector<GraphLane> apply(Log *log)
    {
        if (log->subject() == "resolve symbols export errors on msvc") {
            qDebug() << "wip: model";
        }
        int myIndex = -1;
        QVector<GraphLane> lanes = initLanes(hashes, log->commitHash(), myIndex);
        join(log->commitHash(), hashes, lanes, myIndex);
        if (log->childs().size() ) {
//            if (myIndex!=-1)
//                hashes.replace(myIndex, log->childs().first());
            fork(log->childs(), hashes, lanes, !log->parents().size(), myIndex);
        }
        return lanes;
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
    LanesFactory factory;
    for (auto i = rbegin(); i != rend(); i++) {
        auto &log = *i;
        log->_lanes = factory.apply(log);
    }
}

}
