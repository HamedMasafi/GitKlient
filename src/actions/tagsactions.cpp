#include "tagsactions.h"

#include "dialogs/taginfodialog.h"
#include "git/gitmanager.h"

#include <QAction>
#include <QMenu>

#include <klocalizedstring.h>


TagsActions::TagsActions(Git::Manager *git, QWidget *parent) : AbstractActions(git, parent)
{
    ADD_HIDDEN_ACTION(actionCreate, "New tag...", &TagsActions::create);
    ADD_ACTION(actionRemove, "Remove...", &TagsActions::remove);
    ADD_ACTION(actionCheckout, "Checkout...", &TagsActions::checkout);

    _actionRemove->setEnabled(false);
    _actionCheckout->setEnabled(false);
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
}

void TagsActions::create()
{
    TagInfoDialog d(_parent);
    d.setWindowTitle(i18n("New tag"));
    if (d.exec() == QDialog::Accepted) {
        _git->createTag(d.tagName(), d.message());
    }
}

void TagsActions::remove()
{

}

void TagsActions::checkout()
{

}
