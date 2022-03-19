#include "stashactions.h"

#include <KLocalizedString>
#include <KMessageBox>

#include <QMenu>
#include <QDebug>

#include "diffwindow.h"
#include "git/gitmanager.h"

StashActions::StashActions(Git::Manager *git, QWidget *parent) : AbstractActions(git, parent)
{
    ADD_ACTION(actionDiff, "Diff with HEAD...", &StashActions::diff);
    ADD_ACTION(actionPop, "Pop...", &StashActions::pop);
    ADD_ACTION(actionApply, "Apply...", &StashActions::apply);
    ADD_ACTION(actionDrop, "Drop...", &StashActions::drop);

    _actionApply->setEnabled(false);
    _actionDiff->setEnabled(false);
    _actionDrop->setEnabled(false);
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
