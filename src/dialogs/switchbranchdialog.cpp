#include "switchbranchdialog.h"
#include "git/commands/commandswitchbranch.h"
#include "git/gitmanager.h"
#include "runnerdialog.h"
#include "git/models/branchescache.h"
#include "git/models/tagsmodel.h"

#include <QDebug>

SwitchBranchDialog::SwitchBranchDialog(Git::Manager *git, QWidget *parent) : Dialog(git, parent)
{
    setupUi(this);
    comboBoxBranchSelect->setModel(git->branchesModel());
    comboBoxTags->setModel(git->tagsModel());
}

void SwitchBranchDialog::on_buttonBox_accepted()
{
    Git::CommandSwitchBranch cmd(_git);

    if (radioButtonExistingBranch->isChecked()) {
        cmd.setMode(Git::CommandSwitchBranch::ExistingBranch);
        cmd.setTarget(comboBoxBranchSelect->currentText());
    } else if (radioButtonTag->isChecked()) {
        cmd.setMode(Git::CommandSwitchBranch::Tag);
        cmd.setTarget(comboBoxTags->currentText());
    } else {
        cmd.setMode(Git::CommandSwitchBranch::NewBranch);
        cmd.setTarget(lineEditNewBranchName->text());
    }

    qDebug() << cmd.target();
    return;

    cmd.setForce(checkBoxForce->isChecked());

    RunnerDialog d(this);
    d.run(&cmd);
    d.exec();

    accept();
}