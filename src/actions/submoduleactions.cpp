#include "submoduleactions.h"
#include "dialogs/submoduleinfodialog.h"

#include <QAction>
#include <QMenu>

#include <klocalizedstring.h>


SubmoduleActions::SubmoduleActions(Git::Manager *git, QWidget *parent)
    : AbstractActions(git, parent)
{
    ADD_HIDDEN_ACTION(actionCreate, "Create...", &SubmoduleActions::create);
    ADD_ACTION(actionInit, "Init...", &SubmoduleActions::init);
    ADD_ACTION(actionUpdate, "Update...", &SubmoduleActions::update);
    ADD_ACTION(actionDeinit, "Deinit...", &SubmoduleActions::deinit);
    ADD_ACTION(actionSync, "Sync...", &SubmoduleActions::sync);

    _actionInit->setEnabled(false);
    _actionUpdate->setEnabled(false);
    _actionDeinit->setEnabled(false);
    _actionSync->setEnabled(false);
}

void SubmoduleActions::init()
{

}

void SubmoduleActions::update()
{

}

void SubmoduleActions::create()
{
    SubmoduleInfoDialog d(_parent);
    if (d.exec() == QDialog::Accepted) {
    }
}

void SubmoduleActions::deinit()
{

}

void SubmoduleActions::sync()
{

}
