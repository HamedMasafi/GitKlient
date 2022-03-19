#include "changedfileactions.h"
#include "git/gitmanager.h"
#include "diffwindow.h"

#include <QAction>
#include <QMenu>

#include <KLocalizedString>

ChangedFileActions::ChangedFileActions(Git::Manager *git, QWidget *parent)
    : AbstractActions(git, parent)
{
    ADD_ACTION(actionDiff, "Diff...", &ChangedFileActions::diff);
    ADD_ACTION(actionRevert, "Revert...", &ChangedFileActions::revert);

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
