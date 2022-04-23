#include "submoduleactions.h"
#include "dialogs/submoduleinfodialog.h"
#include "dialogs/runnerdialog.h"
#include "git/commands/addsubmodulecommand.h"
#include "git/models/submodulescache.h"
#include "git/gitmanager.h"

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

    setActionEnabled(_actionInit, true);
    setActionEnabled(_actionUpdate, true);
    setActionEnabled(_actionDeinit, true);
    setActionEnabled(_actionSync, true);
}

SubmoduleActions::SubmoduleActions(Git::Manager *git, QWidget *parent)
    : AbstractActions(git, parent)
{
    _actionCreate = addActionHidden(i18n("Add..."), this, &SubmoduleActions::create);
    _actionInit = addAction(i18n("Init..."), this, &SubmoduleActions::init, false);
    _actionUpdate = addAction(i18n("Update..."), this, &SubmoduleActions::update, false);
    _actionDeinit = addAction(i18n("Deinit..."), this, &SubmoduleActions::deinit, false);
    _actionSync = addAction(i18n("Sync..."), this, &SubmoduleActions::sync, false);

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
        RunnerDialog runner;
        runner.run(d.command());
        runner.exec();
        _git->submodulesModel()->load();
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
