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
    return 3;
}

QVariant BranchesCache::data(const QModelIndex &index, int role) const
{
    if (role != Qt::DisplayRole || !index.isValid() || index.row() < 0
        || index.row() >= _data.size())
        return QVariant();

    switch (index.column()) {
    case 0:
        return _data.at(index.row())->name;
    case 1:
        return _data.at(index.row())->commitsBehind;
    case 2:
        return _data.at(index.row())->commitsAhead;
    }

    return QVariant();
}

QVariant BranchesCache::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
        return QVariant();

    switch (section) {
    case 0:
        return "Name";
    case 1:
        return "Commit(s) behind";
    case 2:
        return "Commit(s) ahead";
    }
    return QVariant();
}

BranchesCache::BranchData *BranchesCache::fromindex(const QModelIndex &index) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= _data.size())
        return nullptr;

    return _data.at(index.row());
}

void BranchesCache::fill()
{
    qDeleteAll(_data);
    _data.clear();

    QStringList branchesList;
    auto out = _git->readAllNonEmptyOutput({"branch", "--list"});

    for (auto &line : out) {
        auto b = line.trimmed();
        if (b.isEmpty())
            continue;
        if (b.startsWith("* ")) {
            b = b.mid(2);
            _referenceBranch = _currentBranch = b.trimmed();
        }

        branchesList.append(b.trimmed());

        auto branch = new BranchData;
        branch->name = b.trimmed();
        branch->commitsAhead = branch->commitsBehind = 0;
        _data.append(branch);
    }
    calculateCommitStats();
}

const QString &BranchesCache::referenceBranch() const
{
    return _referenceBranch;
}

void Git::BranchesCache::calculateCommitStats()
{
    for (auto &b: _data) {
        auto commitsInfo = _git->uniqueCommiteOnBranches(_referenceBranch, b->name);
        b->commitsBehind = commitsInfo.first;
        b->commitsAhead = commitsInfo.second;
    }
    emit dataChanged(index(0, 1), index(_data.size() - 1, 2));
}

void BranchesCache::setReferenceBranch(const QString &newReferenceBranch)
{
    _referenceBranch = newReferenceBranch;

    calculateCommitStats();
}

const QString &BranchesCache::currentBranch() const
{
    return _currentBranch;
}

} // namespace Git
