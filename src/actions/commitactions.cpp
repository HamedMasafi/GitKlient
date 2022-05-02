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

    setActionEnabled(_actionBrowse, true);
    setActionEnabled(_actionCheckout, true);
    setActionEnabled(_actionDiff, true);
}

CommitActions::CommitActions(Git::Manager *git, QWidget *parent)
: AbstractActions(git, parent)
{
    _actionBrowse = addAction(i18n("Browse..."),this, &CommitActions::browse,  false, true);
    _actionCheckout = addAction(i18n("Checkout..."),this, &CommitActions::checkout,  false, true);
    _actionDiff = addAction(i18n("Diff with HEAD..."),this, &CommitActions::diff,  false, true);
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
    auto d = new DiffWindow(_git, _commitHash, "HEAD");
    d->showModal();
}
