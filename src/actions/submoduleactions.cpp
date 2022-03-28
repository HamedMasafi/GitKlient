#include "submoduleactions.h"
#include "dialogs/submoduleinfodialog.h"
#include "dialogs/runnerdialog.h"

#include <QAction>
#include <QMenu>

#include <klocalizedstring.h>



const QString &SubmoduleActions::subModuleName() const
{
    return _subModuleName;
}

void SubmoduleActions::setSubModuleName(const QString &newSubModuleName)
{
    _subModuleName = newSubModuleName;

    _actionInit->setEnabled(true);
    _actionUpdate->setEnabled(true);
    _actionDeinit->setEnabled(true);
    _actionSync->setEnabled(true);
}

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

    _actionCreate->setIcon(QIcon::fromTheme("list-add"));
}

void SubmoduleActions::init()
{
    RunnerDialog d(_parent);
    d.run({"submodule", "init", _subModuleName});
    d.exec();
}

void SubmoduleActions::update()
{
    RunnerDialog d(_parent);
    d.run({"submodule", "update", _subModuleName});
    d.exec();
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
    RunnerDialog d(_parent);
    d.run({"submodule", "sync", _subModuleName});
    d.exec();
}
