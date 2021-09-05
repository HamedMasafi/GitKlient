#include "filesmodel.h"

FilesModel::FilesModel(QObject *parent) : QAbstractListModel(parent)
{

}

int FilesModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return _files.size();
}

int FilesModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 2;
}

QVariant FilesModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole || index.row() < 0
        || index.row() >= _files.size())
        return QVariant();

    auto row = _files[index.row()];

    if (index.column() == 1)
        return row.second;

    return row.first;
}

void FilesModel::append(const QString &data)
{
    auto i = data.lastIndexOf("/");
    if (i < data.size() - 1)
        _files.append({data.mid(i + 1), data});
    else
        _files.append({data, data});
}
