#include "tagsactions.h"

#include "dialogs/runnerdialog.h"
#include "dialogs/taginfodialog.h"
#include "git/gitmanager.h"
#include "git/models/tagsmodel.h"

#include <KMessageBox>
#include <QAction>
#include <QMenu>

#include "diffwindow.h"
#include <klocalizedstring.h>


TagsActions::TagsActions(Git::Manager *git, QWidget *parent) : AbstractActions(git, parent)
{
    _actionCreate = addActionHidden(i18n("New tag..."), this, &TagsActions::create);
    _actionRemove = addActionDisabled(i18n("Remove..."), this, &TagsActions::remove);
    _actionCheckout = addActionDisabled(i18n("Checkout..."), this, &TagsActions::checkout);
    _actionDiff = addActionDisabled(i18n("Diff with HEAD..."), this, &TagsActions::diff);
    _actionPush = addAction(i18n("Push..."), this, &TagsActions::push);
}

const QString &TagsActions::tagName() const
{
    return _tagName;
}

void TagsActions::setTagName(const QString &newTagName)
{
    _tagName = newTagName;

    setActionEnabled(_actionRemove, true);
    setActionEnabled(_actionCheckout, true);
    setActionEnabled(_actionDiff, true);
}

void TagsActions::create()
{
    TagInfoDialog d(_parent);
    d.setWindowTitle(i18n("New tag"));
    if (d.exec() == QDialog::Accepted) {
        _git->createTag(d.tagName(), d.message());
        _git->tagsModel()->load();
    }
}

void TagsActions::remove()
{
    auto r = KMessageBox::questionYesNo(_parent, i18n("Are you sure to remove the selected tag?"));

    if (r == KMessageBox::No)
        return;

    _git->runGit({"tag", "-d", _tagName});
    _git->tagsModel()->load();
}

void TagsActions::checkout()
{
    auto r = KMessageBox::questionYesNo(_parent, i18n("Are you sure to restore to the the selected tag?"));

    if (r == KMessageBox::No)
        return;

    _git->runGit({"tag", "checkout", "tags/" + _tagName});
}

void TagsActions::diff()
{
    auto d = new DiffWindow(_tagName, "HEAD");
    d->showModal();
}

void TagsActions::push()
{
    RunnerDialog d(_parent);
    d.run({"push", "--tags"});
    d.exec();
}
