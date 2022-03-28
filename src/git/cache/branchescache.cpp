#include "branchescache.h"

#include "../gitmanager.h"

namespace Git {

BranchesCache::BranchesCache(Manager *git, QObject *parent)
    : Cache{git, parent}
{

}

int BranchesCache::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return _data.size();
}

int BranchesCache::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 1;
}

QVariant BranchesCache::data(const QModelIndex &index, int role) const
{
    if (role != Qt::DisplayRole || !index.isValid() || index.row() < 0
        || index.row() >= _data.size())
        return QVariant();

    return _data.at(index.row());
}

void BranchesCache::fill()
{
    _data.clear();

    QStringList branchesList;
    auto out = _git->readAllNonEmptyOutput({"branch", "--list"});

    for (auto &line : out) {
        auto b = line.trimmed();
        if (b.isEmpty())
            continue;
        if (b.startsWith("* ")) {
            b = b.mid(2);
            _currentBranch = b.trimmed();
        }

        branchesList.append(b.trimmed());
    }
}

} // namespace Git
