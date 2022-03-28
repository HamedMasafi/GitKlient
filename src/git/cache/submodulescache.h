#ifndef SUBMODULESCACHE_H
#define SUBMODULESCACHE_H

#include "cache.h"

namespace Git {

class Submodule;
class SubmodulesCache : public Cache
{
    Q_OBJECT

    QList<Submodule *> _data;

    enum Role {
        PathRole = Qt::UserRole + 1,
        HeadRole,
        StatusRole
    };

public:
    explicit SubmodulesCache(Git::Manager *git, QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;

    bool append(Submodule *module);

protected:
    void fill() override;
};

} // namespace Git

#endif // SUBMODULESCACHE_H
