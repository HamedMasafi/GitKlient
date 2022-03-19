#include "commitactions.h"
#include "dialogs/filestreedialog.h"
#include "dialogs/runnerdialog.h"
#include "diffwindow.h"

#include <QAction>
#include <QMenu>

#include <KLocalizedString>



const QString &CommitActions::commitHash() const
{
    return _commitHash;
}

void CommitActions::setCommitHash(const QString &newCommitHash)
{
    _commitHash = newCommitHash;
}

CommitActions::CommitActions(Git::Manager *git, QWidget *parent)
: AbstractActions(git, parent)
{
    ADD_ACTION(actionBrowse, "Browse...", &CommitActions::browse);
    ADD_ACTION(actionCheckout, "Checkout...", &CommitActions::checkout);
    ADD_ACTION(actionDiff, "Diff with HEAD...", &CommitActions::diff);
}

void CommitActions::browse()
{
    FilesTreeDialog d(_commitHash, _parent);
    d.exec();
}

void CommitActions::checkout()
{
    RunnerDialog d(_parent);
    d.run({"checkout", _commitHash});
    d.exec();
}

void CommitActions::diff()
{
    auto d = new DiffWindow(_commitHash, "HEAD");
    d->showModal();
}
