#include "treeview.h"

TreeView::TreeView(QWidget *parent) : QTreeView(parent)
{

}

void TreeView::currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(current);
    emit itemActivated(current);
}
