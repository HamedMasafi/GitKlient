#include "difftreemodel.h"

#include <QDebug>
#include <GitKlientSettings.h>

DiffTreeModel::DiffTreeModel(QObject *parent) : TreeModel(parent)
{
    setLastPartAsData(true);
}

void DiffTreeModel::addFile(const FileStatus &file)
{
    TreeModel::Node *node{nullptr};

    auto nodePath = file.name();

    auto parts = nodePath.split(seprator());
    node = createPath(parts, toDiffType(file.status()));
    node->key = file.name();
}

void DiffTreeModel::addFile(const QString &file, const Diff::DiffType &type)
{
    TreeModel::Node *node{nullptr};

    auto parts = file.split(seprator());
    node = createPath(parts, type);
    node->key = file;
    node->metaData = type;
}

TreeModel::Node *DiffTreeModel::createPath(const QStringList &path, const Diff::DiffType &status)
{
    Node *parent = rootNode;
    for (auto &p: path) {
        auto child = parent->find(p);
        if (!child) {
            child = parent->createChild();
            child->title = p;
            child->metaData = Diff::DiffType::Unchanged;
        } else {
            if (status != Diff::DiffType::Unchanged && child->metaData != status)
                child->metaData = Diff::DiffType::Modified;
        }
        parent = child;
    }
    return parent;
}

QString icon(const Diff::DiffType &status)
{
    switch (status) {
    case  Diff::DiffType::Added:
        return "git-status-add";
    case  Diff::DiffType::Modified:
        return "git-status-modified";
    case  Diff::DiffType::Removed:
        return "git-status-remove";
    case  Diff::DiffType::Unchanged:
        return "git-status-update";
    }
    return "git-status-update";
}

QColor textColor(const Diff::DiffType &status)
{
    switch (status) {
    case  Diff::DiffType::Added:
        return GitKlientSettings::self()->diffAddedColor();
    case  Diff::DiffType::Modified:
        return GitKlientSettings::self()->diffModifiedColor();
    case  Diff::DiffType::Removed:
        return GitKlientSettings::self()->diffRemovedColor();
    case  Diff::DiffType::Unchanged:
        return QColor();
    }
    return QColor();
}

QColor DiffTreeModel::statusColor(const Diff::DiffType &status) const
{
    switch (status) {
    case Diff::DiffType::Unchanged:
        return Qt::black;
    case Diff::DiffType::Added:
        return Qt::green;
    case Diff::DiffType::Removed:
        return Qt::red;
    case Diff::DiffType::Modified:
        return Qt::blue;
    }
    return QColor();
}

Diff::DiffType DiffTreeModel::toDiffType(const FileStatus::Status &status)
{
    switch (status) {
    case FileStatus::Unknown:
    case FileStatus::Unmodified:
        return Diff::DiffType::Unchanged;
    case FileStatus::Added:
        return Diff::DiffType::Added;
    case FileStatus::Removed:
        return Diff::DiffType::Removed;
    case FileStatus::Modified:
    case FileStatus::Renamed:
    case FileStatus::Copied:
        return Diff::DiffType::Modified;
    case FileStatus::UpdatedButInmerged:
    case FileStatus::Ignored:
    case FileStatus::Untracked:
        return Diff::DiffType::Unchanged;
    }
    return Diff::DiffType::Unchanged;
}

QVariant DiffTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();


    if (role == Qt::DisplayRole) {
        Node *item = static_cast<Node*>(index.internalPointer());

        switch (index.column() ) {
        case 0:
            return item->title;
        case 1:
            return (int)item->metaData;
        }
    } else if (role == Qt::DecorationRole) {
        Node *item = static_cast<Node*>(index.internalPointer());

        //        return statusColor(item->metaData);
        qDebug() << "icon" << item->title << QIcon::fromTheme(icon(item->metaData)).isNull();
        return QIcon::fromTheme(icon(item->metaData));
    } else if (role == Qt::ForegroundRole) {
        Node *item = static_cast<Node *>(index.internalPointer());
        return textColor(item->metaData);
    }

    return QVariant();
}

void DiffTreeModel::emitAll()
{
    beginInsertRows(QModelIndex(), 0, 1);
    endInsertRows();
}
