#include "stashactions.h"

#include <KLocalizedString>
#include <KMessageBox>

#include <QMenu>
#include <QDebug>
#include <QInputDialog>

#include "diffwindow.h"
#include "git/gitmanager.h"
#include "git/models/stashesmodel.h"

StashActions::StashActions(Git::Manager *git, QWidget *parent) : AbstractActions(git, parent)
{
    _actionNew = addActionHidden(i18n("New stash..."), this, &StashActions::create);
    _actionDiff = addAction(i18n("Diff with HEAD..."),this, &StashActions::diff,  false, true);
    _actionPop = addAction(i18n("Pop..."),this, &StashActions::pop,  false, true);
    _actionApply = addAction(i18n("Apply..."),this, &StashActions::apply,  false, true);
    _actionDrop = addAction(i18n("Remove..."),this, &StashActions::drop,  false, true);

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

    setActionEnabled(_actionApply, true);
    setActionEnabled(_actionDiff, true);
    setActionEnabled(_actionDrop, true);
    setActionEnabled(_actionDrop, true);
}

void StashActions::apply()
{
    auto r = KMessageBox::questionYesNo(_parent,
                                        i18n("Are you sure to apply the selected stash?"),
                                        i18n("Apply stash %1", _stashName));

    if (r == KMessageBox::Yes)
        _git->applyStash(_stashName);
}

void StashActions::drop()
{
    auto r = KMessageBox::questionYesNo(_parent, i18n("Are you sure to apply the selected stash?"), i18n("Apply stash %1", _stashName));

    if (r == KMessageBox::Yes) {
        _git->removeStash(_stashName);
        _git->stashesModel()->load();
    }
}

void StashActions::pop()
{
    auto r = KMessageBox::questionYesNo(_parent,
                                        i18n("Are you sure to apply the selected stash?"),
                                        i18n("Apply stash %1", _stashName));

    if (r == KMessageBox::Yes) {
        _git->runGit({"stash", "push", _stashName});
        _git->stashesModel()->load();
    }
}

void StashActions::diff()
{
    auto d = new DiffWindow(_git, _stashName, "HEAD");
    d->showModal();
}

void StashActions::create()
{
    if (_git->changedFiles().empty()) {
        KMessageBox::information(_parent, i18n("You don't have any changes!"), i18n("Stash"));
        return;
    }
    bool ok;

    auto name = QInputDialog::getText(_parent,
                          i18n("Create new stash"),
                          i18n("Name of stash"),
                          QLineEdit::Normal,
                          QString(),
                          &ok);

    if (ok) {
        _git->createStash(name);
    }
}
