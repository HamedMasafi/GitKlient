#include "submodulescache.h"
#include "../gitsubmodule.h"
#include "../gitmanager.h"

namespace Git {

SubmodulesCache::SubmodulesCache(Git::Manager *git, QObject *parent)
    : Cache{git, parent}
{

}

int SubmodulesCache::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return _data.size();
}

int SubmodulesCache::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 3;
}

QVariant SubmodulesCache::data(const QModelIndex &index, int role) const
{
    if (role != Qt::DisplayRole || !index.isValid() || index.row() < 0
        || index.row() >= _data.size())
        return QVariant();

    auto submodule = _data.at(index.row());

    switch (index.column()) {
    case PathRole:
        return submodule->path();
        break;
    }
    return QVariant();
}

bool SubmodulesCache::append(Submodule *module)
{
    beginInsertRows(QModelIndex(), _data.size(), _data.size());
    _data.append(module);
    endInsertRows();
    return true;
}

void SubmodulesCache::fill()
{
    qDeleteAll(_data);
    _data.clear();
    auto modulesList = _git->readAllNonEmptyOutput({"submodule", "status"});
    for (auto &line : modulesList) {
        auto m = new Submodule;
        m->setCommitHash(line.mid(0, 40));
        auto n = line.lastIndexOf(" ");
        m->setPath(line.mid(41, n - 41));
        m->setRefName(line.mid(n));
        _data.append(m);
    }
}

} // namespace Git
