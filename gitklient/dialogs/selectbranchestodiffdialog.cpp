#include "selectbranchestodiffdialog.h"

#include "git/gitmanager.h"

SelectBranchesToDiffDialog::SelectBranchesToDiffDialog(Git::Manager *git, QWidget *parent) :
      QDialog(parent)
{
    setupUi(this);

    auto branches = git->branches();

    comboBoxOldBranch->addItems(branches);
    comboBoxNewBranch->addItems(branches);
}

QString SelectBranchesToDiffDialog::oldBranch() const
{
    return comboBoxOldBranch->currentText();
}

QString SelectBranchesToDiffDialog::newBranch() const
{
    return comboBoxNewBranch->currentText();
}
