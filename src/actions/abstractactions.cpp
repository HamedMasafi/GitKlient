#include "abstractactions.h"

#include <QMenu>
#include <QWidget>

AbstractActions::AbstractActions(Git::Manager *git, QWidget *parent)
    : QObject{parent}, _git{git} , _parent{parent}
{
    _menu = new QMenu(parent);
}

void AbstractActions::popup()
{
    _menu->popup(QCursor::pos());
}

void AbstractActions::popup(const QPoint &pos)
{
    _menu->popup(pos);
}
