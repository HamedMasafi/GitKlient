#include "stasheswidget.h"
#include "git/gitmanager.h"
#include "actions/stashactions.h"

#include <QInputDialog>
#include <QDebug>

#include <KLocalizedString>
#include <QStandardItemModel>

StashesWidget::StashesWidget(QWidget *parent) :
      WidgetBase(parent)
{
    setupUi(this);
    _actions = new StashActions(Git::Manager::instance(), this);
}

StashesWidget::StashesWidget(Git::Manager *git, GitKlientWindow *parent) : WidgetBase(git, parent)
{
    setupUi(this);
    _actions = new StashActions(git, this);

    pushButtonApply->setAction(_actions->actionApply());
    pushButtonRemoveSelected->setAction(_actions->actionDrop());
//    pushButtonApply->addAction(_actions->actionApply());
}

void StashesWidget::reload()
{
    listWidget->clear();
    _stashes = git()->stashes();
    model = new QStandardItemModel(this);
    model->setRowCount(_stashes.size());
    model->setColumnCount(4);
    model->setHeaderData(0, Qt::Horizontal, i18n("Subject"));
    model->setHeaderData(1, Qt::Horizontal, i18n("Author name"));
    model->setHeaderData(2, Qt::Horizontal, i18n("Author email"));
    model->setHeaderData(3, Qt::Horizontal, i18n("Time"));

    int id{0};
    for (const auto &stash: qAsConst(_stashes)) {
        listWidget->addItem(stash.name());
        model->setItem(id, 0, new QStandardItem(stash.subject()));
        model->setItem(id, 1, new QStandardItem(stash.authorName()));
        model->setItem(id, 2, new QStandardItem(stash.authorEmail()));
        model->setItem(id, 3, new QStandardItem(stash.pushTime().toString()));
        id++;
    }
    treeView->setModel(model);
//    listWidget->addItems(git()->stashes());
}

void StashesWidget::on_pushButtonCreateNew_clicked()
{
    bool ok;
    QInputDialog d;
    auto name = d.getText(this,
                          i18n("Create new stash"),
                          i18n("Name of stash"),
                          QLineEdit::Normal,
                          QString(),
                          &ok);

    if (ok) {
        git()->createStash(name);
        reload();
    }
}


void StashesWidget::on_pushButtonApply_clicked()
{
    auto stashName = listWidget->currentItem()->text();
    git()->applyStash(stashName);
}


void StashesWidget::on_pushButtonRemoveSelected_clicked()
{
    auto stashName = listWidget->currentItem()->text();
    git()->removeStash(stashName);
}


void StashesWidget::on_listWidget_itemActivated(QListWidgetItem *item)
{
    Q_UNUSED(item)

    pushButtonApply->setEnabled(true);
    pushButtonRemoveSelected->setEnabled(true);
}

void StashesWidget::on_listWidget_customContextMenuRequested(const QPoint &pos)
{
    _actions->setStashName(listWidget->currentItem()->text());
    _actions->popup(listWidget->mapToGlobal(pos));
}

void StashesWidget::on_treeView_customContextMenuRequested(const QPoint &pos)
{
    auto stash = _stashes.at(treeView->currentIndex().row());

    _actions->setStashName(stash.name());
    _actions->popup();
}

void StashesWidget::saveState(QSettings &settings) const
{
    save(settings, treeView);
}

void StashesWidget::restoreState(QSettings &settings)
{
    restore(settings, treeView);
}

