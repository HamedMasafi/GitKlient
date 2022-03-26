//
// Created by hamed on 25.03.22.
//

#ifndef GITKLIENT_REMOTESCACHE_H
#define GITKLIENT_REMOTESCACHE_H

#include "cache.h"
#include <QAbstractListModel>

namespace Git {
    class Remote;

    class RemotesCache :/* public QAbstractListModel, */public Cache {
        Q_OBJECT
        QList<Remote*> _data;

    public:
//        int columnCount(const QModelIndex &parent) const override;
//        int rowCount(const QModelIndex &parent) const override;
//        QVariant data(const QModelIndex &index, int role) const override;
    };
}

#endif //GITKLIENT_REMOTESCACHE_H
