#ifndef STASHESCACHE_H
#define STASHESCACHE_H

#include "cache.h"

namespace Git {

class Manager;
class Stash;
class StashesCache : public Cache
{
    Q_OBJECT
    QList<Stash *> _data;

public:
    explicit StashesCache(Manager *git, QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

protected:
    void fill() override;
};

}

#endif // STASHESCACHE_H
