#include "submoduleactions.h"
#include "dialogs/submoduleinfodialog.h"
#include "dialogs/runnerdialog.h"
#include "git/commands/addsubmodulecommand.h"
#include "git/models/submodulesmodel.h"
#include "git/gitmanager.h"

#include <QDebug>
#include <QAction>
#include <QMenu>

#include <kmessagebox.h>
#include <klocalizedstring.h>
#include <QDir>



const QString &SubmoduleActions::subModuleName() const
{
    return _subModuleName;
}

void SubmoduleActions::setSubModuleName(const QString &newSubModuleName)
{
    _subModuleName = newSubModuleName.trimmed();

    setActionEnabled(_actionInit, true);
    setActionEnabled(_actionUpdate, true);
//    setActionEnabled(_actionDeinit, true);
    setActionEnabled(_actionSync, true);
}

SubmoduleActions::SubmoduleActions(Git::Manager *git, QWidget *parent)
    : AbstractActions(git, parent)
{
    _actionCreate = addActionHidden(i18n("Add..."), this, &SubmoduleActions::create);
    _actionInit = addAction(i18n("Init..."), this, &SubmoduleActions::init, false);
    _actionUpdate = addAction(i18n("Update..."), this, &SubmoduleActions::update, false);
//    _actionDeinit = addAction(i18n("Remove..."), this, &SubmoduleActions::deinit, false);
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
    SubmoduleInfoDialog d(_git, _parent);
    if (d.exec() == QDialog::Accepted) {
        RunnerDialog runner;
        runner.run(d.command());
        runner.exec();
        _git->submodulesModel()->load();
    }
}

void SubmoduleActions::deinit()
{
    auto r = KMessageBox::questionYesNo(_parent, i18n("Are you sure to remove the selected submodule?"));

    if (r == KMessageBox::No)
        return;

    qDebug() << _git->runGit({"submodule", "deinit", "-f", "--", _subModuleName});
    qDebug()<<_git->runGit({"rm", _subModuleName});

    QDir d(_git->path() + "/.git/modules/" + _subModuleName);
    if (!d.removeRecursively()) {
        KMessageBox::sorry(_parent, i18n("Unable to remove the module directory"));
        return;
    }
    qDebug()<<d.path();
    _git->runGit({"config", "--remove-section", "submodule." + _subModuleName});

    _git->submodulesModel()->load();
    KMessageBox::information(_parent, i18n("The submodule %1 removed", _subModuleName));
}

void SubmoduleActions::sync()
{
    RunnerDialog d(_parent);
    d.run({"submodule", "sync", _subModuleName});
    d.exec();
}
