#ifndef FILESMODEL_H
#define FILESMODEL_H

#include "git/filestatus.h"
#include <QtCore/QAbstractListModel>

#include <QList>
#include <QPair>
#include <QString>

class FilesModel: public QAbstractListModel
{
    Q_OBJECT

    QList<QPair<QString, QString>> _files;

public:
    explicit FilesModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    void append(const QString &data);

    void addFile(const FileStatus &file);
};

#endif // FILESMODEL_H
