#include "dialogs/submoduleinfodialog.h"
#include "submoduleswidget.h"
#include "git/gitmanager.h"
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
}

SubmodulesWidget::SubmodulesWidget(Git::Manager *git, GitKlientWindow *parent) :
      WidgetBase(git, parent)
{
    setupUi(this);
    _actions = new SubmoduleActions(git, this);

    pushButtonAddNew->setAction(_actions->actionCreate());
    pushButtonUpdate->setAction(_actions->actionUpdate());
}

void SubmodulesWidget::saveState(QSettings &settings) const
{
    save(settings, treeWidget);
}

void SubmodulesWidget::restoreState(QSettings &settings)
{
    restore(settings, treeWidget);
}

void SubmodulesWidget::reload()
{
    treeWidget->clear();
    auto modulesList = git()->submodules();

    auto changedFiles = git()->changedFiles();

    for (auto &m: modulesList) {
        auto item = new QTreeWidgetItem(treeWidget);
        item->setText(0, m.path());
        item->setText(1, m.refName());

        auto status = changedFiles.value(m.path());
        qDebug() << "status for" << m.path() << status;

        treeWidget->addTopLevelItem(item);
    }
}

void SubmodulesWidget::on_treeWidget_customContextMenuRequested(const QPoint &pos)
{
    Q_UNUSED(pos)

    if (!treeWidget->currentItem())
        return;

    _actions->setSubModuleName(treeWidget->currentItem()->text(0));
    _actions->popup();
}

void SubmodulesWidget::on_treeWidget_itemActivated(QTreeWidgetItem *item, int)
{
    qDebug() << item->text(0);
    _actions->setSubModuleName(item->text(0));
}
