#ifndef HISTORYMODEL_H
#define HISTORYMODEL_H

#include <QAbstractListModel>
#include <QDateTime>
#include "git/gitmanager.h"
#include "git/gitloglist.h"

class HistoryModel : public QAbstractListModel
{
    Q_OBJECT
    QString _branch;
public:
    Q_DECL_DEPRECATED
    explicit HistoryModel(QObject *parent = nullptr);
    const QString &branch() const;
    void setBranch(const QString &newBranch);

    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    Git::Log *log(const QModelIndex &index) const;
    QModelIndex findIndexByHash(const QString &hash) const;
    Git::Log *findLogByHash(const QString &hash) const;

public Q_SLOTS:
    void reload();

private:
    Git::LogList _logs;
};

#endif // HISTORYMODEL_H
