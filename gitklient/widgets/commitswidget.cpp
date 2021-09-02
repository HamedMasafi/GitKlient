#include "commitswidget.h"

#include "dialogs/filestreedialog.h"
#include "dialogs/diffdialog.h"
#include "models/treemodel.h"
#include "git/gitmanager.h"

#include <QAction>
#include <QMenu>

CommitsWidget::CommitsWidget(QWidget *parent) : WidgetBase(parent)
{
    setupUi(this);
    init();
}

CommitsWidget::CommitsWidget(Git::Manager *git, QWidget *parent) : WidgetBase(git, parent)
{
    setupUi(this);
    init();
}

void CommitsWidget::reload()
{
    _repoModel->clear();
    _repoModel->addData(git()->branches());
}

void CommitsWidget::on_treeViewRepo_activated(const QModelIndex &index)
{
    auto key = _repoModel->key(index);
    if (!key.isEmpty())
        widgetCommitsView->setBranch(key);
}

void CommitsWidget::on_treeViewRepo_customContextMenuRequested(const QPoint &pos)
{
    _branchesMenu->popup(treeViewRepo->mapToGlobal(pos));
}

void CommitsWidget::on_actionBrowse_triggered()
{
    auto branchName = _repoModel->fullPath(treeViewRepo->currentIndex());
    FilesTreeDialog d(branchName, this);
    d.exec();
}

void CommitsWidget::on_actionDiffWithMain_triggered()
{
    auto branchName = _repoModel->fullPath(treeViewRepo->currentIndex());
    DiffDialog d(branchName, "master", this);
    d.exec();
}

void CommitsWidget::init()
{
    _repoModel = new TreeModel(this);
    treeViewRepo->setModel(_repoModel);

    _branchesMenu = new QMenu(this);
    _branchesMenu->addAction(actionBrowse);
    _branchesMenu->addAction(actionDiffWithMain);
}
