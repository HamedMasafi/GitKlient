#include "stasheswidget.h"
#include "git/gitmanager.h"
#include "git/models/stashescache.h"
#include "actions/stashactions.h"

#include <QInputDialog>
#include <QDebug>

#include <KLocalizedString>
#include <QStandardItemModel>

StashesWidget::StashesWidget(QWidget *parent) :
      WidgetBase(parent)
{
    setupUi(this);
    init(Git::Manager::instance());
}


StashesWidget::StashesWidget(Git::Manager *git, GitKlientWindow *parent) : WidgetBase(git, parent)
{
    setupUi(this);
    init(git);
}

void StashesWidget::init(Git::Manager *git)
{
    _actions = new StashActions(git, this);

    pushButtonApply->setAction(_actions->actionApply());
    pushButtonRemoveSelected->setAction(_actions->actionDrop());
    pushButtonCreateNew->setAction(_actions->actionNew());

    _model = git->stashesCache();
    treeView->setModel(_model);
}

void StashesWidget::reload()
{
//    _stashes = git()->stashes();
//    model->clear();
//    model->setColumnCount(4);
//    model->setRowCount(_stashes.size());

//    model->setHeaderData(0, Qt::Horizontal, i18n("Subject"));
//    model->setHeaderData(1, Qt::Horizontal, i18n("Author name"));
//    model->setHeaderData(2, Qt::Horizontal, i18n("Author email"));
//    model->setHeaderData(3, Qt::Horizontal, i18n("Time"));

//    int id{0};
//    for (const auto &stash: qAsConst(_stashes)) {
//        model->setItem(id, 0, new QStandardItem(stash.subject()));
//        model->setItem(id, 1, new QStandardItem(stash.authorName()));
//        model->setItem(id, 2, new QStandardItem(stash.authorEmail()));
//        model->setItem(id, 3, new QStandardItem(stash.pushTime().toString()));
//        id++;
//    }

}

void StashesWidget::on_treeView_customContextMenuRequested(const QPoint &pos)
{
    Q_UNUSED(pos)

    auto stash = _model->fromIndex(treeView->currentIndex());

    if (!stash)
        return;

    _actions->setStashName(stash->name());
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

