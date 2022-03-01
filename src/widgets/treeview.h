#ifndef TREEVIEW_H
#define TREEVIEW_H

#include <QTreeView>

class TreeView : public QTreeView
{
    Q_OBJECT
public:
    TreeView(QWidget *parent = nullptr);

protected slots:
    void currentChanged(const QModelIndex &current, const QModelIndex &previous) override;

signals:
    void itemActivated(const QModelIndex &index);
};

#endif // TREEVIEW_H
