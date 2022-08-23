#include "commitswidget.h"

#include "actions/branchactions.h"
#include "dialogs/filestreedialog.h"
#include "models/treemodel.h"
#include "git/gitmanager.h"

#include <QAction>
#include <QMenu>

CommitsWidget::CommitsWidget(QWidget *parent) : WidgetBase(parent)
{
    setupUi(this);
    init();
}

CommitsWidget::CommitsWidget(Git::Manager *git, AppWindow *parent) : WidgetBase(git, parent)
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
    widgetCommitsView->saveState(settings);
}

void CommitsWidget::restoreState(QSettings &settings)
{
    restore(settings, splitter);
    restore(settings, treeViewRepo);
    widgetCommitsView->restoreState(settings);
}

void CommitsWidget::on_treeViewRepo_itemActivated(const QModelIndex &index)
{
    auto key = _repoModel->key(index);
    if (!key.isEmpty())
        widgetCommitsView->setBranch(key);
}

void CommitsWidget::on_treeViewRepo_customContextMenuRequested(const QPoint &pos)
{
    Q_UNUSED(pos)
    auto branchName = _repoModel->fullPath(treeViewRepo->currentIndex());
    _actions->setBranchName(branchName);
    _actions->popup();
}

void CommitsWidget::init()
{
    _repoModel = new TreeModel(this);
    treeViewRepo->setModel(_repoModel);

    _actions = new BranchActions(_git, this);
}
