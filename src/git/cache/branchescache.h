#ifndef BRANCHESCACHE_H
#define BRANCHESCACHE_H

#include "cache.h"

#include <QStringList>

namespace Git {

class Manager;
class BranchesCache : public Cache
{
    Q_OBJECT

    QStringList _data;
    QString _currentBranch;

public:
    explicit BranchesCache(Manager *git, QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;

protected:
    void fill() override;
};

} // namespace Git

#endif // BRANCHESCACHE_H
