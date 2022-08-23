#include "treewidget.h"

TreeWidget::TreeWidget(QWidget *parent) : QTreeWidget(parent)
{

}

void TreeWidget::currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous)
    emit itemActivated(current);
}
