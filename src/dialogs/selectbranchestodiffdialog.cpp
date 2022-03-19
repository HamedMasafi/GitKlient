#include "selectbranchestodiffdialog.h"

#include "git/gitmanager.h"
#include <KMessageBox>
#include <KLocalizedString>

SelectBranchesToDiffDialog::SelectBranchesToDiffDialog(Git::Manager *git, QWidget *parent) :
      Dialog(parent)
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

void SelectBranchesToDiffDialog::on_buttonBox_accepted()
{
    if (oldBranch() == newBranch()) {
        KMessageBox::sorry(this, i18n("The selected branches must be different!"));
        return;
    }
    accept();
}
