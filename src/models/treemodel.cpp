#include "treemodel.h"

#include <QColor>
#include <QModelIndex>
#include <QDebug>

#include <KLocalizedString>

const QString &TreeModel::separator() const
{
    return _seprator;
}

void TreeModel::setSeparator(const QString &newSeparator)
{
    _seprator = newSeparator;
}

bool TreeModel::lastPartAsData() const
{
    return _lastPartAsData;
}

void TreeModel::setLastPartAsData(bool newLastPartAsData)
{
    _lastPartAsData = newLastPartAsData;
}

const QIcon &TreeModel::defaultIcon() const
{
    return _defaultIcon;
}

void TreeModel::setDefaultIcon(const QIcon &newDefaultIcon)
{
    _defaultIcon = newDefaultIcon;
}

void TreeModel::clear()
{
    beginRemoveRows(QModelIndex(), 0, rootNode->childs.count() - 1);
    qDeleteAll(rootNode->childs);
    rootNode->childs.clear();
    endRemoveRows();
}

TreeModel::TreeModel(QObject *parent) : QAbstractItemModel(parent), rootNode(new Node)
{
}

QModelIndex TreeModel::index(const Node *node, int col) const
{
    if (node->parent)
        return index(node->row, col, index(node->parent, col));

    return index(node->row, col, QModelIndex());
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
    Node *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootNode;
    else
        parentItem = static_cast<Node*>(parent.internalPointer());

    return parentItem->childs.count();
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 1;
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();


    if (role == Qt::DisplayRole && index.column() == 0) {
        Node *item = static_cast<Node*>(index.internalPointer());

        return item->title;
    } else if (role == Qt::DecorationRole) {
        return _defaultIcon;
    }

    return QVariant();
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    Node *parentItem;

    if (!parent.isValid())
        parentItem = rootNode;
    else
        parentItem = static_cast<Node*>(parent.internalPointer());

    Node *childItem = parentItem->childs.at(row);
    if (childItem)
        return createIndex(row, column, childItem);
    return {};
}

QModelIndex TreeModel::parent(const QModelIndex &child) const
{
    if (!child.isValid())
        return {};

    Node *childItem = static_cast<Node*>(child.internalPointer());
    Node *parentItem = childItem->parent;

    if (parentItem == rootNode)
        return {};

    return createIndex(parentItem->row, 0, parentItem);
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        if (section == 0)
            return i18n("Name");
        else
            return i18n("Status");
    } else {
        return section + 1;
    }
}

QStringList TreeModel::rootData() const
{
    return rootNode->data;
}

QStringList TreeModel::data(const QModelIndex &index) const
{
    return static_cast<Node*>(index.internalPointer())->data;
}

QString TreeModel::fullPath(const QModelIndex &index) const
{
    QString path;

    if (index.isValid())
        getFullPath(path, static_cast<Node *>(index.internalPointer()));
    else
        getFullPath(path, rootNode);

    return path;
}

QString TreeModel::key(const QModelIndex &index) const
{
    auto node = static_cast<Node *>(index.internalPointer());
    if (node)
        return node->key;
    return QString();
}

QString TreeModel::section(const QModelIndex &index) const
{
    auto node = static_cast<Node *>(index.internalPointer());
    if (node)
        return node->prefix;
    return QString();
}

void TreeModel::sortItems()
{
    sortNode(rootNode);
}


void TreeModel::addData(const QStringList &data, const QString &prefix, bool split)
{
    for (auto &p: data) {
        auto path = p;
        path = path.replace("\r", "").replace("\n", "").trimmed();
        if (path.isEmpty())
            continue;

        TreeModel::Node *node;

        if (split) {
            auto nodePath = path;
            if (prefix != QString())
                nodePath.prepend(prefix + _seprator);

            auto parts = nodePath.split(_seprator);
            if (_lastPartAsData) {
                auto data = parts.takeLast();
                node = createPath(parts);
                node->data.append(data);
                auto nodePathParts = nodePath.split(_seprator);
                nodePathParts.takeLast();
                node->key = nodePathParts.join(_seprator);
            } else {
                node = createPath(parts);
                node->key = path;
            }
        } else {
            if (prefix != QString())
                node = createPath({prefix, path});
            else
                node = createPath({path});
            node->key = path;
        }
        if (node) {
            node->prefix = prefix;
        }
    }
    beginInsertRows(QModelIndex(), 0, rootNode->childs.count() - 1);
    endInsertRows();
}

TreeModel::Node *TreeModel::find(QStringList &path, Node *node)
{
    if (path.empty())
        return nullptr;

    auto ch = node->find(path.first());
    if (!ch)
        return nullptr;

    auto p = path;
    p.removeFirst();
    return find(p, ch);
}

TreeModel::Node *TreeModel::createPath(const QStringList &path)
{
    Node *parent = rootNode;
    for (auto &p: path) {
        auto child = parent->find(p);
        if (!child) {
            child = parent->createChild();
            child->title = p;
        }
        parent = child;
    }
    return parent;
}

void TreeModel::getFullPath(QString &path, Node *node) const
{
    if (node) {
        path.prepend(node->title);

        if (node->parent != rootNode) {
            path.prepend(_seprator);
            getFullPath(path, node->parent);
        }
    }
}

void TreeModel::sortNode(Node *node)
{
    qDebug() << "Sorting" << node->title;
    std::sort(node->childs.begin(), node->childs.end(), [](Node *l, Node *r) {
        if (l->childs.empty() && !r->childs.empty())
            return false;
        if (!l->childs.empty() && r->childs.empty())
            return true;
        return l->title < r->title;
    });
    for (auto &n: node->childs)
        sortNode(n);
}
