#include "switchbranchdialog.h"
#include "git/commands/commandswitchbranch.h"

SwitchBranchDialog::SwitchBranchDialog(Git::Manager *git, QWidget *parent) : Dialog(git, parent)
{
    setupUi(this);
}

void SwitchBranchDialog::on_buttonBox_accepted()
{
    accept();
}
