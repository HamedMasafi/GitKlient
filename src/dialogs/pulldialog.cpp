#include "pulldialog.h"
#include "runnerdialog.h"

#include "git/gitmanager.h"
#include "git/commands/commandpull.h"

PullDialog::PullDialog(QWidget *parent, Git::Manager *git) :
      AppDialog(parent)
{
    setupUi(this);

    auto g = git;
    if(!g)
        g = Git::Manager::instance();
    comboBoxRemote->addItems(g->remotes());
    comboBoxBranch->addItems(g->branches());

    comboBoxRemote->setCurrentText(g->currentBranch());

    //git pull --squash --no-ff --ff-only --no-commit --prune --tags
}

void PullDialog::on_buttonBox_accepted()
{
    Git::CommandPull cmd;

    cmd.setRemote(comboBoxRemote->currentText());
    cmd.setBranch(comboBoxBranch->currentText());
    cmd.setSquash(checkBoxSquash->isChecked());
    cmd.setNoFf(checkBoxNoFastForward->isChecked());
    cmd.setFfOnly(checkBoxFastForwardOnly->isChecked());
    cmd.setNoCommit(checkBoxNoCommit->isChecked());
    cmd.setPrune(checkBoxPrune->isChecked());
    cmd.setTags(checkBoxTags->isChecked());

    RunnerDialog d(this);
    d.run(&cmd);
    d.exec();

    accept();
}
