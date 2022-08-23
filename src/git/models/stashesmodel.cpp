#include "stashesmodel.h"

#include "../gitmanager.h"

#include <QDebug>

//#include <klocalizedstring.h>

#define i18n(x) x

namespace Git {

StashesModel::StashesModel(Manager *git, QObject *parent) : AbstractGitItemsModel(git, parent) {}

int StashesModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return _data.size();
}

int StashesModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 4;
}

QVariant StashesModel::data(const QModelIndex &index, int role) const
{
    if (role != Qt::DisplayRole || !index.isValid() || index.row() < 0
        || index.row() >= _data.size())
        return QVariant();

    auto remote = _data.at(index.row());

    switch (index.column()) {
    case 0:
        return remote->subject();
    case 1:
        return remote->authorName();
    case 2:
        return remote->authorEmail();
    case 3:
        return remote->pushTime();
    }
    return QVariant();
}

QVariant StashesModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal)
        switch (section){
            case 0: return i18n("Subject");
            case 1: return i18n("Author name");
            case 2: return i18n("Author email");
            case 3: return i18n("Time");
        }

    return QVariant();
}

Stash *StashesModel::fromIndex(const QModelIndex &index) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= _data.size())
        return nullptr;

    return _data.at(index.row());
}

void StashesModel::fill()
{
    qDeleteAll(_data);
    _data.clear();

    auto list = _git->readAllNonEmptyOutput({"stash", "list", "--format=format:%s%m%an%m%ae%m%aD"});
    int id{0};
    for (const auto &item: qAsConst(list)) {
        auto parts = item.split(">");
        if (parts.size() != 4)
            continue;

        auto subject = parts.first();
        auto stash = new Stash(_git, QStringLiteral("stash@{%1}").arg(id));

        stash->_subject = subject;
        stash->_authorName = parts.at(1);
        stash->_authorEmail = parts.at(2);
        stash->_pushTime = QDateTime::fromString(parts.at(3), Qt::RFC2822Date);

        _data.append(stash);
        id++;
    }
}

}
