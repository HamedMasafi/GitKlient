#include "tagsmodel.h"
#include "../gittag.h"
#include "../gitmanager.h"

#include <QDebug>

#include <klocalizedstring.h>

namespace Git {

TagsModel::TagsModel(Manager *git, QObject *parent) : AbstractGitItemsModel(git, parent) {}

int TagsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return _data.size();
}

int TagsModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 3;
}

QVariant TagsModel::data(const QModelIndex &index, int role) const
{
    if (role != Qt::DisplayRole || !index.isValid() || index.row() < 0
        || index.row() >= _data.size())
        return QVariant();

    auto remote = _data.at(index.row());

    switch (index.column()) {
    case 0:
        return remote->name();
    case 1:
        return remote->message();
    case 2:
        return remote->taggerEmail();
    }
    return QVariant();
}

QVariant TagsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal)
        switch (section){
        case 0: return i18n("Name");
        case 1: return i18n("Subject");
        case 2: return i18n("Author email");
        }

    return QVariant();
}

Tag *TagsModel::fromIndex(const QModelIndex &index) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= _data.size())
        return nullptr;

    return _data.at(index.row());
}

void TagsModel::fill()
{
    qDeleteAll(_data);
    _data.clear();
    auto list = _git->readAllNonEmptyOutput({"--no-pager","tag", "--list", "--format=%(subject)>%(tag)>%(taggername)"});
    qDebug() << list;
    for (auto &i: list) {
        auto parts = i.split(">");
        if (parts.size() != 3)
            continue;
        auto tag = new Tag;
        tag->setMessage(parts.at(0));
        tag->setName(parts.at(1));
        tag->setTaggerEmail(parts.at(2));
        _data.append(tag);
    }

}

} // namespace Git
