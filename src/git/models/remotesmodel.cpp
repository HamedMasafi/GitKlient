//
// Created by hamed on 25.03.22.
//

#include "remotesmodel.h"
#include "../gitremote.h"
#include "../gitmanager.h"

namespace Git {

RemotesModel::RemotesModel(Manager *git, QObject *parent) : AbstractGitItemsModel(git, parent)
{

}

int RemotesModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 3;
}

int RemotesModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return _data.count();
}

QVariant RemotesModel::data(const QModelIndex &index, int role) const
{
    if (role != Qt::DisplayRole || !index.isValid() || index.row() < 0
        || index.row() >= _data.size())
        return QVariant();

    auto remote = _data.at(index.row());

    switch (index.column()) {
    case 0:
        return remote->name;
    }
    return QVariant();
}

Remote *RemotesModel::fromIndex(const QModelIndex &index)
{
    if (!index.isValid() || index.row() < 0 || index.row() >= _data.size())
        return nullptr;

    return _data.at(index.row());
}

Remote *RemotesModel::findByName(const QString &name)
{
    for(const auto &d: qAsConst(_data))
        if (d->name == name)
            return d;
    return nullptr;
}

void RemotesModel::rename(const QString &oldName, const QString &newName)
{
    _git->runGit({"remote", "rename", oldName, newName});
    load();
}

void RemotesModel::setUrl(const QString &remoteName, const QString &newUrl)
{
    _git->runGit({"remote", "set-url", remoteName, newUrl});
    load();
}

void RemotesModel::fill()
{
    qDeleteAll(_data.begin(), _data.end());
    _data.clear();

    if (!_git)
        return;

    auto remotes = _git->remotes();
    for (const auto &remote: qAsConst(remotes)) {
        auto r = new Remote;
        r->name = remote;
        auto ret = QString(_git->runGit({"remote", "show", remote}));
        r->parse(ret);
        _data.append(r);
    }
}

}
