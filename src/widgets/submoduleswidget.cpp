#include "dialogs/submoduleinfodialog.h"
#include "submoduleswidget.h"
#include "git/gitmanager.h"
#include "git/models/submodulesmodel.h"
#include "actions/submoduleactions.h"

#include <QDebug>
#include <QListWidgetItem>

SubmodulesWidget::SubmodulesWidget(QWidget *parent) :
      WidgetBase(parent)
{
    setupUi(this);
    _actions = new SubmoduleActions(Git::Manager::instance(), this);

    pushButtonAddNew->setAction(_actions->actionCreate());
    pushButtonUpdate->setAction(_actions->actionUpdate());

    _model = Git::Manager::instance()->submodulesModel();
    treeView->setModel(_model);
}

SubmodulesWidget::SubmodulesWidget(Git::Manager *git, AppWindow *parent) :
      WidgetBase(git, parent)
{
    setupUi(this);
    _actions = new SubmoduleActions(git, this);

    pushButtonAddNew->setAction(_actions->actionCreate());
    pushButtonUpdate->setAction(_actions->actionUpdate());

    _model = git->submodulesModel();
    treeView->setModel(_model);
}

void SubmodulesWidget::saveState(QSettings &settings) const
{
    save(settings, treeView);
}

void SubmodulesWidget::restoreState(QSettings &settings)
{
    restore(settings, treeView);
}

void SubmodulesWidget::reload()
{

}

void SubmodulesWidget::on_treeView_customContextMenuRequested(const QPoint &pos)
{
    Q_UNUSED(pos)

    if (!treeView->currentIndex().isValid())
        return;

    auto s = _model->fromIndex(treeView->currentIndex());
    if (!s)
        return;
    _actions->setSubModuleName(s->path());
    _actions->popup();
}

void SubmodulesWidget::on_treeView_activated(const QModelIndex &index)
{
    auto s = _model->fromIndex(index);
    if (!s)
        return;
    _actions->setSubModuleName(s->path());
}
