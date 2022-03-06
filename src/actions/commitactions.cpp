#include "commitactions.h"

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
}

void CommitActions::browse()
{

}
