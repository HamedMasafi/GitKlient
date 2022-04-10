#include "abstractactions.h"

#include "git/gitmanager.h"

#include <klocalizedstring.h>
#include <QDebug>
#include <QMenu>
#include <QWidget>

void AbstractActions::setActionEnabled(QAction *action, bool enabled)
{
    if (_git->isValid()) {
        action->setEnabled(enabled);
    } else {
        action->setEnabled(false);
        _actionStatuses.insert(action, enabled);
    }
}

AbstractActions::AbstractActions(Git::Manager *git, QWidget *parent)
    : QObject{parent}, _git{git} , _parent{parent}
{
    _menu = new QMenu(parent);
    connect(git, &Git::Manager::pathChanged, this, &AbstractActions::git_reloaded);
}

void AbstractActions::popup()
{
    _menu->popup(QCursor::pos());
}

void AbstractActions::popup(const QPoint &pos)
{
    _menu->popup(pos);
}

void AbstractActions::git_reloaded()
{
    if (!_git->isValid()) {
        for (auto &a: _actions)
            a->setEnabled(false);
        _actionStatuses.clear();
        return;
    }

    for (auto i = _actionStatuses.begin(); i != _actionStatuses.end(); ++i) {
        i.key()->setEnabled(i.value());
    }

    _actionStatuses.clear();
}

QAction *AbstractActions::createAction(const QString &text, bool enabled, bool addToMenu)
{
    auto a = new QAction(this);
    a->setText(text);
    setActionEnabled(a, enabled);
    if (addToMenu)
        _menu->addAction(a);
    _actions.append(a);
    return a;
}
