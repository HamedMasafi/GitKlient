//
// Created by hamed on 25.03.22.
//

#include "remotescache.h"
#include "../gitremote.h"
#include "../gitmanager.h"

namespace Git {

RemotesCache::RemotesCache(Manager *git, QObject *parent) : Cache(git, parent)
{

}

int RemotesCache::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 3;
}

int RemotesCache::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return _data.count();
}

QVariant RemotesCache::data(const QModelIndex &index, int role) const
{
    if (role != Qt::DisplayRole || !index.isValid() || index.row() < 0
        || index.row() >= _data.size())
        return QVariant();

    auto remote = _data.at(index.row());

    switch (index.column()) {
    case 0:
        return remote->name;
        break;
    }
    return QVariant();
}

Remote *RemotesCache::fromIndex(const QModelIndex &index)
{
    if (!index.isValid() || index.row() < 0 || index.row() >= _data.size())
        return nullptr;

    return _data.at(index.row());
}

void RemotesCache::fill()
{
    qDeleteAll(_data.begin(), _data.end());
    _data.clear();

    if (!_git)
        return;

    auto remotes = _git->remotes();
    for (const auto &remote: qAsConst(remotes)) {
        auto r = new Remote;
        auto ret = QString(_git->runGit({"remote", "show", remote}));
        r->parse(ret);
        _data.append(r);
    }
}

}
