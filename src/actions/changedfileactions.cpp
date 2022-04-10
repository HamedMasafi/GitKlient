#include "changedfileactions.h"
#include "git/gitmanager.h"
#include "diffwindow.h"

#include <QAction>
#include <QMenu>

#include <KLocalizedString>

ChangedFileActions::ChangedFileActions(Git::Manager *git, QWidget *parent)
    : AbstractActions(git, parent)
{
    _actionDiff = addAction(i18n("Diff..."),this, &ChangedFileActions::diff,  false, true);
    _actionRevert = addAction(i18n("Revert..."),this, &ChangedFileActions::revert,  false, true);

    auto f = _actionDiff->font();
    f.setBold(true);
    _actionDiff->setFont(f);
}

const QString &ChangedFileActions::filePath() const
{
    return _filePath;
}

void ChangedFileActions::setFilePath(const QString &newFilePath)
{
    _filePath = newFilePath;

    setActionEnabled(_actionDiff, true);
    setActionEnabled(_actionRevert, true);
}

void ChangedFileActions::diff()
{
    Git::File original{_git->currentBranch(), _filePath};
    Git::File changed{_git->path() + "/" + _filePath};

    auto diffWin = new DiffWindow(original, changed);
    diffWin->showModal();
}

void ChangedFileActions::revert()
{
    _git->revertFile(_filePath);
    emit reloadNeeded();
}
