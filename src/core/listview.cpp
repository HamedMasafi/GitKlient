#include "listview.h"

ListView::ListView(QWidget *parent) : QListView(parent)
{

}

void ListView::currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous)
    emit itemActivated(current);
}
