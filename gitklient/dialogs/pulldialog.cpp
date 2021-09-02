#include "pulldialog.h"

#include "git/gitmanager.h"
#include "git/commandpull.h"

PullDialog::PullDialog(QWidget *parent, Git::Manager *git) :
      QDialog(parent)
{
    setupUi(this);

    auto g = git;
    if(!g)
        g = Git::Manager::instance();
    comboBoxRemote->addItems(g->remotes());
    comboBoxBranch->addItems(g->branches());

    comboBoxRemote->setCurrentText(g->currentBranch());

    Git::CommandPull cmd;

    cmd.setSquash(checkBoxSquash->isChecked());
    cmd.setNoFf(checkBoxNoFastForward->isChecked());
    cmd.setFfOnly(checkBoxFastForwardOnly->isChecked());
    cmd.setNoCommit(checkBoxNoCommit->isChecked());
    cmd.setPrune(checkBoxPrune->isChecked());
    cmd.setTags(checkBoxTags->isChecked());

    //git pull --squash --no-ff --ff-only --no-commit --prune --tags
}
