#include "branchactions.h"

#include <KLocalizedString>
#include <kmessagebox.h>

#include <QAction>
#include <QFileDialog>
#include <QMenu>

#include "dialogs/filestreedialog.h"
#include "dialogs/runnerdialog.h"
#include "diffwindow.h"
#include "git/gitmanager.h"

const QString &BranchActions::branchName() const
{
    return _branchName;
}

void BranchActions::setBranchName(const QString &newBranchName)
{
    _branchName = newBranchName;
}

const QString &BranchActions::otherBranch() const
{
    return _otherBranch;
}

void BranchActions::setOtherBranch(const QString &newOtherBranch)
{
    _otherBranch = newOtherBranch;
}

void BranchActions::create()
{

}

BranchActions::BranchActions(Git::Manager *git, QWidget *parent) : AbstractActions(git, parent)
{
    ADD_ACTION(actionBrowse, "Browse...", &BranchActions::browse);
    ADD_ACTION(actionCheckout, "Checkout...", &BranchActions::checkout);
    ADD_ACTION(actionDiff, "Diff...", &BranchActions::diff);
    ADD_ACTION(actionRemove, "Remove...", &BranchActions::remove);
    ADD_HIDDEN_ACTION(actionCreate, "Create...", &BranchActions::create);
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

    auto d = new DiffWindow(mainBranch, _branchName);
    d->showModal();
}

void BranchActions::remove()
{
    auto r = KMessageBox::questionYesNo(_parent, i18n("Are you sure to remove the selected branch?"));

    if (r == KMessageBox::No)
        return;

    _git->removeBranch(_branchName);
}
