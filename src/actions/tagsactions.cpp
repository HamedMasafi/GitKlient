#include "tagsactions.h"

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
    ADD_HIDDEN_ACTION(actionCreate, "New tag...", &TagsActions::create);
    ADD_ACTION(actionRemove, "Remove...", &TagsActions::remove);
    ADD_ACTION(actionCheckout, "Checkout...", &TagsActions::checkout);
    ADD_ACTION(actionDiff, "Diff with HEAD...", &TagsActions::diff);

    _actionRemove->setEnabled(false);
    _actionCheckout->setEnabled(false);
    _actionDiff->setEnabled(false);
}

const QString &TagsActions::tagName() const
{
    return _tagName;
}

void TagsActions::setTagName(const QString &newTagName)
{
    _tagName = newTagName;

    _actionRemove->setEnabled(true);
    _actionCheckout->setEnabled(true);
    _actionDiff->setEnabled(true);
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
