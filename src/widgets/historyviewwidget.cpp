#include "historyviewwidget.h"
#include "models/historymodel.h"
#include "dialogs/filestreedialog.h"
#include "git/gitlog.h"
#include <QDebug>
#include <QMenu>
#include "widgets/graphpainter.h"
#include <dialogs/diffdialog.h>

HistoryViewWidget::HistoryViewWidget(QWidget *parent) :
      WidgetBase(parent)
{
    setupUi(this);
    _historyModel = new HistoryModel(this);
    treeViewHistory->setModel(_historyModel);

    _graphPainter = new GraphPainter(_historyModel, this);
    treeViewHistory->setItemDelegateForColumn(0, _graphPainter);

    _commitMenu = new QMenu(this);
    auto action = _commitMenu->addAction(tr("Browse"));
    connect(action, &QAction::triggered, this, &HistoryViewWidget::browseCommit);

    connect(Git::Manager::instance(),
            &Git::Manager::pathChanged,
            this,
            &HistoryViewWidget::git_pathChanged);
}

HistoryViewWidget::HistoryViewWidget(Git::Manager *git, QWidget *parent):
      WidgetBase(git, parent)
{
    setupUi(this);
    _historyModel = new HistoryModel(this);
    treeViewHistory->setModel(_historyModel);

    _graphPainter = new GraphPainter(_historyModel, this);
    treeViewHistory->setItemDelegateForColumn(0, _graphPainter);

    _commitMenu = new QMenu(this);
    auto action = _commitMenu->addAction(tr("Browse"));
    connect(action, &QAction::triggered, this, &HistoryViewWidget::browseCommit);

    connect(Git::Manager::instance(),
            &Git::Manager::pathChanged,
            this,
            &HistoryViewWidget::git_pathChanged);
}

void HistoryViewWidget::setBranch(const QString &branchName)
{
    treeViewHistory->setItemDelegateForColumn(0, nullptr);
    _historyModel->setBranch(branchName);
    if (_historyModel->rowCount(QModelIndex()))
        treeViewHistory->setCurrentIndex(_historyModel->index(0));
}

void HistoryViewWidget::on_treeViewHistory_activated(const QModelIndex &index)
{
    auto log = _historyModel->log(index);
    if (!log)
        return;

    textBrowser->setLog(log);
}

void HistoryViewWidget::on_textBrowser_hashClicked(const QString &hash)
{
    auto index = _historyModel->findIndexByHash(hash);
    if (index.isValid()) {
        treeViewHistory->setCurrentIndex(index);
        on_treeViewHistory_activated(index);
    }
}

void HistoryViewWidget::on_textBrowser_fileClicked(const QString &file)
{
    auto log = textBrowser->log();

    Git::File oldFile;
    Git::File newFile(log->commitHash(), file);
    if (log->parentHash().size()) {
        oldFile = {log->parentHash().first(), file};
    }
    DiffDialog d(oldFile, newFile, this);
    d.exec();
}

void HistoryViewWidget::browseCommit()
{
    auto log = _historyModel->log(treeViewHistory->currentIndex());
    if (!log)
        return;
    FilesTreeDialog d(log->commitHash(), this);
    d.exec();
}

void HistoryViewWidget::on_treeViewHistory_customContextMenuRequested(const QPoint &pos)
{
    _commitMenu->popup(treeViewHistory->mapToGlobal(pos));
}

void HistoryViewWidget::on_treeViewHistory_clicked(const QModelIndex &index)
{
    on_treeViewHistory_activated(index);
}

void HistoryViewWidget::git_pathChanged()
{
    _historyModel->reload();
    if (_historyModel->rowCount(QModelIndex()))
        treeViewHistory->setCurrentIndex(_historyModel->index(0));
}

