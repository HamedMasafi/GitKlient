#include "branchesstatuswidget.h"

#include "actions/branchactions.h"
#include "dialogs/filestreedialog.h"
#include "dialogs/runnerdialog.h"
#include "git/gitmanager.h"
#include "git/models/branchesmodel.h"

BranchesStatusWidget::BranchesStatusWidget(QWidget *parent) : WidgetBase(parent)
{
    setupUi(this);
    init(Git::Manager::instance());
}


BranchesStatusWidget::BranchesStatusWidget(Git::Manager *git, AppWindow *parent) :
      WidgetBase(git, parent)

{
    setupUi(this);
    init(git);
}

void BranchesStatusWidget::init(Git::Manager *git)
{
    _actions = new BranchActions(git, this);
    _model = git->branchesModel();
    treeView->setModel(_model);

    comboBoxReferenceBranch->setModel(_model);

    pushButtonNew->setAction(_actions->actionCreate());
    pushButtonBrowse->setAction(_actions->actionBrowse());
    pushButtonCheckout->setAction(_actions->actionCheckout());
    pushButtonDiff->setAction(_actions->actionDiff());
    pushButtonRemoveSelected->setAction(_actions->actionRemove());
}

void BranchesStatusWidget::saveState(QSettings &settings) const
{
    save(settings, treeView);
}

void BranchesStatusWidget::restoreState(QSettings &settings)
{
    restore(settings, treeView);
}

void BranchesStatusWidget::on_comboBoxReferenceBranch_currentIndexChanged(const QString &selectedBranch)
{
    _model->setReferenceBranch(selectedBranch);
    _actions->setOtherBranch(selectedBranch);
}

void BranchesStatusWidget::on_pushButtonRemoveSelected_clicked()
{
//    if (!treeWidgetBranches->currentItem())
//        return;

//    auto r = KMessageBox::questionYesNo(this, i18n("Are you sure to remove the selected branch?"));

//    if (r == KMessageBox::No)
//        return;

//    git()->removeBranch(treeWidgetBranches->currentItem()->text(0));
//    auto tmp = treeWidgetBranches->takeTopLevelItem(treeWidgetBranches->currentIndex().row());
//    if (tmp)
//        delete tmp;
}

void BranchesStatusWidget::on_treeView_customContextMenuRequested(const QPoint &pos)
{
    Q_UNUSED(pos)
    auto b = _model->fromindex(treeView->currentIndex());
    if (!b)
        return;
    _actions->setBranchName(b->name);
    _actions->popup();
}

void BranchesStatusWidget::reload()
{
    /*comboBoxReferenceBranch->clear();

    _branches = this->git()->branches();
    comboBoxReferenceBranch->addItems(_branches);
    auto index = _branches.indexOf("master");
    if (index != -1) {
        comboBoxReferenceBranch->setCurrentIndex(index);
    } else {
        index = _branches.indexOf("main");
        if (index != -1)
            comboBoxReferenceBranch->setCurrentIndex(index);
    }*/
}
