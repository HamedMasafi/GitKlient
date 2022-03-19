#include "stashactions.h"

#include <KLocalizedString>
#include <KMessageBox>

#include <QMenu>
#include <QDebug>
#include <QInputDialog>

#include "diffwindow.h"
#include "git/gitmanager.h"

StashActions::StashActions(Git::Manager *git, QWidget *parent) : AbstractActions(git, parent)
{
    ADD_ACTION(actionDiff, "Diff with HEAD...", &StashActions::diff);
    ADD_ACTION(actionPop, "Pop...", &StashActions::pop);
    ADD_ACTION(actionApply, "Apply...", &StashActions::apply);
    ADD_ACTION(actionDrop, "Remove...", &StashActions::drop);
    ADD_HIDDEN_ACTION(actionNew, "New stash...", &StashActions::create);

    _actionApply->setEnabled(false);
    _actionDiff->setEnabled(false);
    _actionDrop->setEnabled(false);

    _actionDrop->setIcon(QIcon::fromTheme("list-remove"));
    _actionNew->setIcon(QIcon::fromTheme("list-add"));
}

const QString &StashActions::stashName() const
{
    return _stashName;
}

void StashActions::setStashName(const QString &newStashName)
{
    _stashName = newStashName;

    _actionApply->setEnabled(true);
    _actionDiff->setEnabled(true);
    _actionDrop->setEnabled(true);
}

void StashActions::apply()
{
    auto r = KMessageBox::questionYesNo(_parent, i18n("Are you sure to apply the selected stash?"), i18n("Apply stash %1", _stashName));

    if (r == KMessageBox::Yes)
        _git->applyStash(_stashName);
}

void StashActions::drop()
{
    auto r = KMessageBox::questionYesNo(_parent, i18n("Are you sure to apply the selected stash?"), i18n("Apply stash %1", _stashName));

    if (r == KMessageBox::Yes)
        _git->removeStash(_stashName);
}

void StashActions::pop()
{

}

void StashActions::diff()
{
    qDebug() << "diff" <<_stashName;
    auto d = new DiffWindow(_stashName, "HEAD");
    d->showModal();
}

void StashActions::create()
{
    bool ok;
    QInputDialog d;
    auto name = d.getText(_parent,
                          i18n("Create new stash"),
                          i18n("Name of stash"),
                          QLineEdit::Normal,
                          QString(),
                          &ok);

    if (ok) {
        _git->createStash(name);
    }
}
