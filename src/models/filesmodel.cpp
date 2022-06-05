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
    if (index.row() < 0 || index.row() >= _files.size())
        return QVariant();

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        auto row = _files[index.row()];

        if (index.column() == 1)
            return row.second;
        return row.first;
    }

    return QVariant();
}

void FilesModel::append(const QString &data)
{
    auto i = data.lastIndexOf("/");
    if (i != -1)
        _files.append({data.mid(i + 1), data});
    else
        _files.append({data, data});
}

void FilesModel::addFile(const FileStatus &file)
{

}
