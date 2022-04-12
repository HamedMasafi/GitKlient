//
// Created by hamed on 25.03.22.
//

#ifndef GITKLIENT_REMOTESCACHE_H
#define GITKLIENT_REMOTESCACHE_H

#include "cache.h"
#include <QAbstractListModel>

namespace Git {
class Remote;

class RemotesCache : public Cache
{
    Q_OBJECT
    QList<Remote *> _data;

public:
    RemotesCache(Manager *git, QObject *parent = nullptr);
    int columnCount(const QModelIndex &parent) const override;
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    Remote *fromIndex(const QModelIndex &index);
    Remote *findByName(const QString &name);

    void rename(const QString &oldName, const QString &newName);

protected:
    void fill() override;
};
} // namespace Git

#endif //GITKLIENT_REMOTESCACHE_H
