//
// Created by hamed on 25.03.22.
//

#ifndef GITKLIENT_LOGS_H
#define GITKLIENT_LOGS_H

#include "git/cache/cache.h"
#include <QAbstractListModel>

namespace Git {

class Log;
class Manager;
class LogsCache : public Cache
{
    Q_OBJECT

    QString _branch;
    QList<Log *> _data;
    QStringList _branches;
    QMap<QString, Log *> _dataByCommitHashLong;
    QMap<QString, Log *> _dataByCommitHashShort;

public:
    LogsCache(Manager *git, QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;

protected:
    void fill() override;

private:
    void initChilds();
    void initGraph();
};
}

#endif //GITKLIENT_LOGS_H
