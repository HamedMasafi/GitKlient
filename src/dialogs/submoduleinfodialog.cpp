#include "submoduleinfodialog.h"

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

SubmoduleInfoDialog::SubmoduleInfoDialog(QWidget *parent) :
      Dialog(parent)
{
    setupUi(this);
}
