#include "difftreemodel.h"

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
}

TreeModel::Node *DiffTreeModel::createPath(const QStringList &path, const Diff::DiffType &status)
{
    Node *parent = rootNode;
    for (auto &p: path) {
        auto child = parent->find(p);
        if (!child) {
            child = parent->createChild();
            child->title = p;
            child->metaData = Diff::DiffType::Added;
        } else {
            if (child->metaData != Diff::DiffType::Unchanged)
                child->metaData = status;
        }
        parent = child;
    }
    return parent;
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


    if (role == Qt::DisplayRole && index.column() == 0) {
        Node *item = static_cast<Node*>(index.internalPointer());

        return item->title;
    } else if (role == Qt::DecorationRole) {
        Node *item = static_cast<Node*>(index.internalPointer());

        return statusColor(item->metaData);
    }

    return QVariant();
}
