#include "branchactions.h"

#include <KLocalizedString>
#include <kmessagebox.h>

#include <QAction>
#include <QFileDialog>
#include <QInputDialog>
#include <QMenu>

#include "dialogs/fetchdialog.h"
#include "dialogs/filestreedialog.h"
#include "dialogs/mergedialog.h"
#include "dialogs/notedialog.h"
#include "dialogs/runnerdialog.h"
#include "diffwindow.h"
#include "git/commands/commandmerge.h"
#include "git/gitmanager.h"
#include "git/models/branchesmodel.h"

BranchActions::BranchActions(Git::Manager *git, QWidget *parent) : AbstractActions(git, parent)
{
    _actionCreate = addActionHidden(i18n("Create..."), this, &BranchActions::create);
    _actionCreate->setIcon(QIcon::fromTheme("document-new"));

    _actionFetch = addActionDisabled(i18n("Fetch..."), this, &BranchActions::fetch);
    _actionBrowse = addActionDisabled(i18n("Browse..."), this, &BranchActions::browse);
    _actionCheckout = addActionDisabled(i18n("Switch"), this, &BranchActions::checkout);
    _actionMerge = addActionDisabled(i18n("Merge..."), this, &BranchActions::merge);
    _actionDiff = addActionDisabled(i18n("Diff"), this, &BranchActions::diff);
    _actionRemove = addActionDisabled(i18n("Remove..."), this, &BranchActions::remove);
    _actionNote = addActionDisabled(i18n("Note..."), this, &BranchActions::note);
}

const QString &BranchActions::branchName() const
{
    return _branchName;
}

void BranchActions::setBranchName(const QString &newBranchName)
{
    _branchName = newBranchName;

    setActionEnabled(_actionFetch, true);
    setActionEnabled(_actionBrowse, true);
    setActionEnabled(_actionCheckout, true);
    setActionEnabled(_actionMerge, true);
    setActionEnabled(_actionDiff, true);
    setActionEnabled(_actionRemove, true);
    setActionEnabled(_actionNote, true);
}

const QString &BranchActions::otherBranch() const
{
    return _otherBranch;
}

void BranchActions::setOtherBranch(const QString &newOtherBranch)
{
    _otherBranch = newOtherBranch;

}

void BranchActions::fetch()
{
    FetchDialog d(_git, _parent);
    d.setBranch(_branchName);
    d.exec();
}

void BranchActions::create()
{
    auto newBranchName = QInputDialog::getText(_parent, i18n("Create new branch"), i18n("Branch name"));

    if (newBranchName !=QString()) {
        _git->runGit({"checkout", "-b", newBranchName});
        _git->branchesModel()->load();
    }
}

void BranchActions::browse()
{
    FilesTreeDialog d(_branchName, _parent);
    d.exec();
}

void BranchActions::checkout()
{
    RunnerDialog d(_parent);
    d.run({"checkout", _branchName});
    d.exec();
}

void BranchActions::diff()
{
    QString mainBranch = _otherBranch;

    if (mainBranch == QString()) {
        auto branches = _git->branches();
        if (branches.contains("master"))
            mainBranch = "master";
        else if (branches.contains("main"))
            mainBranch = "main";
        else
            return;
    }

    auto d = new DiffWindow(_git, mainBranch, _branchName);
    d->showModal();
}

void BranchActions::remove()
{
    auto r = KMessageBox::questionYesNo(_parent, i18n("Are you sure to remove the selected branch?"));

    if (r == KMessageBox::No)
        return;

    _git->removeBranch(_branchName);
}

void BranchActions::merge()
{
    MergeDialog d{_git, _branchName, _parent};
    if (d.exec() == QDialog::Accepted) {
        auto cmd = d.command();
        RunnerDialog runner(_parent);
        runner.run(cmd);
        runner.exec();
    }
}

void BranchActions::note()
{
    NoteDialog d{_git, _branchName, _parent};
    d.exec();
}
