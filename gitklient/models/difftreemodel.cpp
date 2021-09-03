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
    node = createPath(parts, file.status());
    node->key = file.name();
}

TreeModel::Node *DiffTreeModel::createPath(const QStringList &path, const FileStatus::Status &status)
{
    Node *parent = rootNode;
    for (auto &p: path) {
        auto child = parent->find(p);
        if (!child) {
            child = parent->createChild();
            child->title = p;
            child->metaData = FileStatus::Added;
        } else {
            if (child->metaData != FileStatus::Unmodified)
                child->metaData = status;
        }
        parent = child;
    }
    return parent;
}

QColor DiffTreeModel::statusColor(const FileStatus::Status &status) const
{
    switch (status) {
    case FileStatus::Unknown:
    case FileStatus::Unmodified:
        return Qt::black;
    case FileStatus::Added:
        return Qt::green;
    case FileStatus::Removed:
        return Qt::red;
    case FileStatus::Modified:
    case FileStatus::Renamed:
    case FileStatus::Copied:
        return Qt::blue;
    case FileStatus::UpdatedButInmerged:
    case FileStatus::Ignored:
    case FileStatus::Untracked:
        break;
    }
    return QColor();
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
