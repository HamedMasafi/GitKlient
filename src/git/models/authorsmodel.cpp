#include "authorsmodel.h"

namespace Git {

AuthorsModel::AuthorsModel(Manager *git, QObject *parent) : AbstractGitItemsModel{git, parent} {}

int AuthorsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return _data.size();
}

int AuthorsModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 2;
}

QVariant AuthorsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
        return QVariant();

    switch (section) {
    case 0:
        return "Name";
    case 1:
        return "Email";
    }
    return QVariant();
}

QVariant AuthorsModel::data(const QModelIndex &index, int role) const
{
    if (role != Qt::DisplayRole || !index.isValid() || index.row() < 0
        || index.row() >= _data.size())
        return QVariant();

    switch (index.column()) {
    case 0:
        return _data.at(index.row())->name;
    case 1:
        return _data.at(index.row())->email;
    }

    return QVariant();
}

Author *AuthorsModel::findOrCreate(const QString &name, const QString &email)
{
    for (auto &a: _data)
        if (a->email == email)
            return a;
    auto author = new Author;
    author->name = name;
    author->email = email;
    beginInsertRows(QModelIndex(), _data.size(), _data.size());
    _data.append(author);
    endInsertRows();
    return author;
}

void AuthorsModel::fill()
{

}

} // namespace Git
