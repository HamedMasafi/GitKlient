#include "submoduleinfodialog.h"

#include "git/commands/addsubmodulecommand.h"

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

SubmoduleInfoDialog::SubmoduleInfoDialog(QWidget *parent) :
      Dialog(parent)
{
    setupUi(this);
}
