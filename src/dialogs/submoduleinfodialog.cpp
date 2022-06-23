#include "submoduleinfodialog.h"

#include "git/commands/addsubmodulecommand.h"
#include "git/gitmanager.h"

#include <klocalizedstring.h>
#include <kmessagebox.h>

#include <QFileDialog>


SubmoduleInfoDialog::SubmoduleInfoDialog(Git::Manager *git, QWidget *parent) :
      AppDialog(git, parent)
{
    setupUi(this);
}

bool SubmoduleInfoDialog::force() const
{
    return checkBoxForce->isChecked();
}

void SubmoduleInfoDialog::setForce(bool newForce)
{
    checkBoxForce->setChecked(newForce);
}

QString SubmoduleInfoDialog::url() const
{
    return lineEditUrl->text();
}

void SubmoduleInfoDialog::setUrl(const QString &newUrl)
{
    lineEditUrl->setText(newUrl);
}

QString SubmoduleInfoDialog::path() const
{
    return lineEditPath->text();
}

void SubmoduleInfoDialog::setPath(const QString &newPath)
{
    lineEditPath->setText(newPath);
}

QString SubmoduleInfoDialog::branch() const
{
    if (checkBoxBranch->isChecked())
        return lineEditBranch->text();
    return QString();
}

void SubmoduleInfoDialog::setBranch(const QString &newBranch)
{
    if (newBranch.isEmpty()) {
        checkBoxBranch->setChecked(true);
        lineEditBranch->setText(newBranch);
    } else {
        checkBoxBranch->setChecked(false);
        lineEditBranch->clear();
    }
}

Git::AddSubmoduleCommand *SubmoduleInfoDialog::command() const
{
    auto cmd = new Git::AddSubmoduleCommand(_git);
    cmd->setForce(checkBoxForce->isChecked());

    if (checkBoxBranch->isChecked())
        cmd->setbranch(lineEditBranch->text());

    cmd->setUrl(lineEditUrl->text());
    cmd->setLocalPath(lineEditPath->text());

    return cmd;
}

void SubmoduleInfoDialog::on_toolButtonBrowseLocalPath_clicked()
{
    auto localPath = QFileDialog::getExistingDirectory(this, i18n("Local path"), _git->path());
    if (localPath.isEmpty())
        return;

    if (!localPath.startsWith(_git->path())) {
        KMessageBox::sorry(this, i18n("The selected path is outside of working dir"));
        return;
    }

    lineEditPath->setText(localPath.replace(_git->path() + "/", ""));
}
