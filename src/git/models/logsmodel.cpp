#include "logsmodel.h"

#include "../gitmanager.h"
#include "../gitlog.h"

namespace Git {

namespace Impl {

void readLine(const QString &line, const QString &seprator, QList<QString *> list)
{
    auto parts = line.split(seprator);
    if (parts.size() != list.size())
        return;

    for (auto i = 0; i < parts.size(); i++)
        *list[i] = parts.at(i);
}
struct LanesFactory {
    QStringList _hashes;

    QList<int> findByChild(const QString &hash)
    {
        int index{0};
        QList<int> ret;
        for (auto const &h : qAsConst(_hashes)) {
            if (hash == h)
                ret.append(index);
            index++;
        }
        return ret;
    }

    int indexOfChild(const QString &hash)
    {
        int index{0};
        for (auto const &h : qAsConst(_hashes)) {
            if (hash == h)
                return index;
            index++;
        }
        return -1;
    }

    QVector<GraphLane> initLanes(const QString &myHash, int &myIndex)
    {
        if (_hashes.empty())
            return QVector<GraphLane>();

        while (!_hashes.empty() && _hashes.last() == QString())
            _hashes.removeLast();

        int index{0};
        QVector<GraphLane> lanes;
        lanes.reserve(_hashes.size());
        for (const auto &hash : qAsConst(_hashes)) {
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

    QList<int> setHashes(const QStringList &children, const int &myIndex)
    {
        QList<int> ret;
        bool myIndexSet{myIndex == -1};
        int index;

        for (const auto &h : children) {
            index = -1;
            if (!myIndexSet) {
                index = myIndex;
                myIndexSet = true;
            }
            if (index == -1)
                index = indexOfChild(QString());

            if (index == -1) {
                _hashes.append(h);
                index = _hashes.size() - 1;
            } else {
                _hashes.replace(index, h);
            }
            ret.append(index);
        }
        return ret;
    }

    void start(const QString &hash, QVector<GraphLane> &lanes)
    {
        Q_UNUSED(hash)
        _hashes.append(QString());
        set(_hashes.size() - 1, GraphLane::Start, lanes);
    }

    void join(const QString &hash, QVector<GraphLane> &lanes, int &myIndex)
    {
        int firstIndex{-1};
        auto list = findByChild(hash);

        for (auto i = list.begin(); i != list.end(); ++i) {
            if (firstIndex == -1) {
                firstIndex = *i;
                set(*i, list.contains(myIndex) ? GraphLane::Node : GraphLane::End, lanes);
            } else {
                auto lane = lanes.at(*i);
                lane._bottomJoins.append(firstIndex);
                lane._type = GraphLane::Transparent;
                set(*i, lane, lanes);
            }
            _hashes.replace(*i, QString());
        }
        myIndex = firstIndex;
    }

    void fork(const QStringList &childrenList, QVector<GraphLane> &lanes, const int &myInedx)
    {
        auto list = setHashes(childrenList, -1);
        auto children = childrenList;
        lanes.reserve(_hashes.size());

        if (myInedx != -1 && lanes.size() <= myInedx)
            lanes.resize(myInedx + 1);

        if (myInedx != -1 && childrenList.size() == 1) {
            auto &l = lanes[list.first()];

            if (list.first() == myInedx) {
                if (l.type() == GraphLane::None)
                    l._type = GraphLane::Transparent;
                if (l.type() == GraphLane::End)
                    l._type = GraphLane::Node;
            } else {
                l._upJoins.append(myInedx);
                lanes[myInedx]._type = GraphLane::End;
            }

            return;
        }
        for (int & i : list) {
            if (lanes.size() <= i)
                lanes.resize(i + 1);

            auto &l = lanes[i];
            if (i == myInedx) {
                l._type = GraphLane::Node;
            } else {
                if (l.type() == GraphLane::None)
                    l._type = GraphLane::Transparent;
                if (l.type() == GraphLane::End)
                    l._type = GraphLane::Node;

                l._upJoins.append(myInedx);
            }
            _hashes.replace(i, children.takeFirst());
        }
    }

    void set(const int &index, const GraphLane &lane, QVector<GraphLane> &lanes)
    {
        if (index < lanes.size())
            lanes.replace(index, lane);
        else
            lanes.append(lane);
    }
    QVector<GraphLane> apply(Log *log)
    {
        int myIndex = -1;
        QVector<GraphLane> lanes = initLanes(log->commitHash(), myIndex);

        if (!log->parents().empty())
            join(log->commitHash(), lanes, myIndex);
        else if (!log->childs().empty()) {
            start(log->childs().first(), lanes);
            myIndex = _hashes.size() - 1;
        }

        if (!log->childs().empty()) {
            fork(log->childs(), lanes, myIndex);
        } else if (myIndex != -1) {
            lanes[myIndex]._type = GraphLane::End;
        }

        return lanes;
    }
};

} // namespace Impl

Git::LogsModel::LogsModel(Manager *git, QObject *parent) : AbstractGitItemsModel(git, parent)
{

}

const QString &LogsModel::branch() const
{
    return _branch;
}

void LogsModel::setBranch(const QString &newBranch)
{
    _branch = newBranch;

    beginResetModel();
    fill();
    endResetModel();
}

int LogsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return _data.size();
}

int LogsModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return _branch.isEmpty() ? 1 : 3;
}

QVariant LogsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Vertical)
        return section + 1;

    if (_branch.isEmpty()) {
        switch (section) {
        case 0:
            return tr("Graph");
        case 1:
            return tr("Message");
        }
    } else {
        switch (section) {
        case 0:
            return tr("Message");
        case 1:
            return tr("Date");
        case 2:
            return tr("Author");
        }
    }
    return QVariant();
}

QVariant LogsModel::data(const QModelIndex &index, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();
    auto log = fromIndex(index);
    if (!log)
        return QVariant();

    if (_branch.isEmpty()) {
        switch (index.column()) {
        case 0:
            return "";
        case 1:
            return log->subject();
        }
    } else {
        switch (index.column()) {
        case 0:
            return log->subject();
        case 1:
            return log->commitDate();
        case 2:
            return log->authorName();
        }
    }

    return QVariant();
}

Log *LogsModel::fromIndex(const QModelIndex &index) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= _data.size())
        return nullptr;

    return _data.at(index.row());
}

QModelIndex LogsModel::findIndexByHash(const QString &hash) const
{
    int idx{0};
    for (auto &log : _data)
        if (log->commitHash() == hash)
            return index(idx);
        else
            idx++;
    return QModelIndex();
}

Log *LogsModel::findLogByHash(const QString &hash) const
{
    int idx{0};
    for (auto &log : _data)
        if (log->commitHash() == hash)
            return log;
        else
            idx++;
    return nullptr;
}

void LogsModel::fill()
{
    qDeleteAll(_data);
    _data.clear();
    _dataByCommitHashLong.clear();

    _branches = _git->branches();

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
        Impl::readLine(lines.at(0), "X", {&d->_commitHash, &d->_commitShortHash, &parentHash});
        Impl::readLine(lines.at(1), "X", {&d->_committerName, &d->_committerEmail, &commitDate});
        Impl::readLine(lines.at(2), "X", {&d->_authorName, &d->_authorEmail, &authDate});

        if (!parentHash.isEmpty())
            d->_parentHash = parentHash.split(" ");
        d->_refLog = lines.at(3);
        d->_subject = lines.at(5);
        d->_commitDate = QDateTime::fromString(commitDate, Qt::ISODate);
        d->_authDate = QDateTime::fromString(authDate, Qt::ISODate);
        d->_body = lines.mid(5).join("\n");
        _data.append(d);
        _dataByCommitHashLong.insert(d->commitHash(), d);
        _dataByCommitHashShort.insert(d->commitShortHash(), d);
    }
    //    std::sort(begin(), end(), [](GitLog *log1,GitLog *log2){
    //        return log1->commitDate() < log2->commitDate();
    //    });
    initChilds();
    initGraph();
}

void LogsModel::initChilds()
{
    for (auto i = _data.rbegin(); i != _data.rend(); i++) {
        auto &log = *i;
        for (auto &p: log->parents())
            _dataByCommitHashLong.value(p)->_childs.append(log->commitHash());
    }
}

void LogsModel::initGraph()
{
    Impl::LanesFactory factory;
    for (auto i = _data.rbegin(); i != _data.rend(); i++) {
        auto &log = *i;
        log->_lanes = factory.apply(log);
    }
}


}
