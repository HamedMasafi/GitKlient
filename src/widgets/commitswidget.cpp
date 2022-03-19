#include "commitswidget.h"

#include "dialogs/filestreedialog.h"
#include "diffwindow.h"
#include "models/treemodel.h"
#include "git/gitmanager.h"

#include <QAction>
#include <QMenu>

CommitsWidget::CommitsWidget(QWidget *parent) : WidgetBase(parent)
{
    setupUi(this);
    init();
}

CommitsWidget::CommitsWidget(Git::Manager *git, GitKlientWindow *parent) : WidgetBase(git, parent)
{
    setupUi(this);
    init();
}

void CommitsWidget::reload()
{
    _repoModel->clear();
    auto branches = git()->branches();
    _repoModel->addData(branches);

    if (branches.contains("master"))
        _mainBranch = "master";
    else if (branches.contains("main"))
        _mainBranch = "main";

    widgetCommitsView->setBranch(QString());
}

void CommitsWidget::saveState(QSettings &settings) const
{
    save(settings, splitter);
    save(settings, treeViewRepo);
}

void CommitsWidget::restoreState(QSettings &settings)
{
    restore(settings, splitter);
    restore(settings, treeViewRepo);
}

void CommitsWidget::on_treeViewRepo_itemActivated(const QModelIndex &index)
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

    auto diffWin = new DiffWindow(branchName, _mainBranch);
    diffWin->showModal();
}

void CommitsWidget::init()
{
    _repoModel = new TreeModel(this);
    treeViewRepo->setModel(_repoModel);

    _branchesMenu = new QMenu(this);
    _branchesMenu->addAction(actionBrowse);
    _branchesMenu->addAction(actionDiffWithMain);

}
