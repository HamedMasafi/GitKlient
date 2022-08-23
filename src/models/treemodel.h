#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QtCore/QAbstractItemModel>

#include <QIcon>

class TreeModel : public QAbstractItemModel
{
    Q_OBJECT
    QString _seprator{"/"};
    bool _lastPartAsData{false};
    QIcon _defaultIcon;

public:
    struct Node
    {
        QString title;
        QList<Node *> childs;
        Node *parent;
        struct Feature *feature;
        int row{};
        QStringList data;
        QString key;
        QString prefix;
#ifdef TREEMODEL_NODE_DATA_TYPE
        TREEMODEL_NODE_DATA_TYPE metaData;
#endif
        Node() : parent(nullptr), feature(nullptr) {}
        explicit Node(Node *parent) : parent(parent), feature(nullptr) { row = parent->childs.count(); }

        ~Node() { qDeleteAll(childs); }

        Node *createChild()
        {
            auto ch = new Node;
            ch->parent = this;
            ch->row = childs.count();
            childs.append(ch);
            return ch;
        }
        Node *find(const QString &title)
        {
            auto i = childs.begin();
            while (i != childs.end()) {
                if ((*i)->title == title)
                    return *i;
                ++i;
            }
            return nullptr;
        }

        void clear()
        {
            qDeleteAll(childs);
            childs.clear();
        }
    };


    explicit TreeModel(QObject *parent = nullptr);

    QModelIndex index(const Node *node, int col) const;
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    QStringList rootData() const;
    QStringList data(const QModelIndex &index) const;
    QString fullPath(const QModelIndex &index) const;
    QString key(const QModelIndex &index) const;
    QString section(const QModelIndex &index) const;
    void sortItems();

    void addData(const QStringList &data, const QString &prefix = QString(), bool split = true);

    const QString &separator() const;
    void setSeparator(const QString &newSeparator);

    bool lastPartAsData() const;
    void setLastPartAsData(bool newLastPartAsData);

    const QIcon &defaultIcon() const;
    void setDefaultIcon(const QIcon &newDefaultIcon);

    void clear();

protected:
    Node *rootNode;

    Node *createPath(const QStringList &path);
    Node *find(QStringList &path, Node *node = nullptr);
    void getFullPath(QString &path, Node *node) const;
    void sortNode(Node *node);
};

#endif // TREEMODEL_H
