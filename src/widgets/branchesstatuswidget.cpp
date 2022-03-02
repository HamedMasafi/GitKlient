#include "branchesstatuswidget.h"

#include "git/gitmanager.h"
#include "dialogs/filestreedialog.h"
#include "dialogs/runnerdialog.h"
#include "diffwindow.h"
#include <KMessageBox>

BranchesStatusWidget::BranchesStatusWidget(QWidget *parent) : WidgetBase(parent)
{
    setupUi(this);
}

BranchesStatusWidget::BranchesStatusWidget(Git::Manager *git, GitKlientWindow *parent) :
      WidgetBase(git, parent)

{
    setupUi(this);
}

void BranchesStatusWidget::saveState(QSettings &settings) const
{
    save(settings, treeWidgetBranches);
}

void BranchesStatusWidget::restoreState(QSettings &settings)
{
    restore(settings, treeWidgetBranches);
}

void BranchesStatusWidget::on_comboBoxReferenceBranch_currentIndexChanged(const QString &selectedBranch)
{
    treeWidgetBranches->clear();
    for (auto &branch : _branches) {
        auto commitsInfo = git()->uniqueCommiteOnBranches(selectedBranch, branch);

        auto item = new QTreeWidgetItem;
        item->setText(0, branch);
        item->setText(1, QString::number(commitsInfo.first));
        item->setText(2, QString::number(commitsInfo.second));
        treeWidgetBranches->addTopLevelItem(item);
    }
}

void BranchesStatusWidget::on_pushButtonRemoveSelected_clicked()
{
    if (!treeWidgetBranches->currentItem())
        return;

    auto r = KMessageBox::questionYesNo(this, i18n("Are you sure to remove the selected branch?"));

    if (r == KMessageBox::No)
        return;

    git()->removeBranch(treeWidgetBranches->currentItem()->text(0));
    auto tmp = treeWidgetBranches->takeTopLevelItem(treeWidgetBranches->currentIndex().row());
    if (tmp)
        delete tmp;
}

void BranchesStatusWidget::on_treeWidgetBranches_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    Q_UNUSED(current)
    Q_UNUSED(previous)

    pushButtonBrowse->setEnabled(true);
    pushButtonCheckout->setEnabled(true);
    pushButtonDiff->setEnabled(true);
    pushButtonRemoveSelected->setEnabled(true);
}

void BranchesStatusWidget::reload()
{
    comboBoxReferenceBranch->clear();

    _branches = this->git()->branches();
    comboBoxReferenceBranch->addItems(_branches);
    auto index = _branches.indexOf("master");
    if (index != -1) {
        comboBoxReferenceBranch->setCurrentIndex(index);
    } else {
        index = _branches.indexOf("main");
        if (index != -1)
            comboBoxReferenceBranch->setCurrentIndex(index);
    }
}


void BranchesStatusWidget::on_pushButtonBrowse_clicked()
{
    if (!treeWidgetBranches->currentItem())
        return;

    FilesTreeDialog d(treeWidgetBranches->currentItem()->text(0), this);
    d.exec();
}


void BranchesStatusWidget::on_pushButtonDiff_clicked()
{
    if (!treeWidgetBranches->currentItem())
        return;

    /*DiffDialog d(treeWidgetBranches->currentItem()->text(0),
                 comboBoxReferenceBranch->currentText(),
                 this);
    d.exec();*/
    auto d = new DiffWindow(treeWidgetBranches->currentItem()->text(0),
                            comboBoxReferenceBranch->currentText());
    d->setWindowModality(Qt::ApplicationModal);
    d->setAttribute(Qt::WA_DeleteOnClose, true);
    d->show();
}


void BranchesStatusWidget::on_pushButtonCheckout_clicked()
{
    RunnerDialog d(this);
    d.run({"checkout", treeWidgetBranches->currentItem()->text(0)});
    d.exec();
}


void BranchesStatusWidget::on_pushButtonNew_clicked()
{

}

