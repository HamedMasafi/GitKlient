#include "selectbranchestodiffdialog.h"

#include "git/gitmanager.h"
#include <KMessageBox>
#include <KLocalizedString>

SelectBranchesToDiffDialog::SelectBranchesToDiffDialog(Git::Manager *git, QWidget *parent) :
      AppDialog(parent)
{
    setupUi(this);

    auto branches = git->branches();

    comboBoxOldBranch->addItems(branches);
    comboBoxNewBranch->addItems(branches);
}

QString SelectBranchesToDiffDialog::oldBranch() const
{
    if (radioButtonLeftIsHead->isChecked())
        return "HEAD";
    return comboBoxOldBranch->currentText();
}

QString SelectBranchesToDiffDialog::newBranch() const
{
    if (radioButtonRightIsHead->isChecked())
        return "HEAD";
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
