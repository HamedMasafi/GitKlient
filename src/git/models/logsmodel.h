//
// Created by hamed on 25.03.22.
//

#ifndef GITKLIENT_LOGS_H
#define GITKLIENT_LOGS_H

#include "abstractgititemsmodel.h"
#include <QAbstractListModel>

namespace Git {

class Log;
class Manager;
class LogsModel : public AbstractGitItemsModel
{
    Q_OBJECT

    QString _branch;
    QList<Log *> _data;
    QStringList _branches;
    QMap<QString, Log *> _dataByCommitHashLong;
    QMap<QString, Log *> _dataByCommitHashShort;

public:
    explicit LogsModel(Manager *git, QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    Log* fromIndex(const QModelIndex &index) const;
    QModelIndex findIndexByHash(const QString &hash) const;
    Git::Log *findLogByHash(const QString &hash) const;

    const QString &branch() const;
    void setBranch(const QString &newBranch);

protected:
    void fill() override;

private:
    void initChilds();
    void initGraph();
};
}

#endif //GITKLIENT_LOGS_H
