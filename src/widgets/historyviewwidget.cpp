#include "historyviewwidget.h"
#include "actions/commitactions.h"
#include "dialogs/filestreedialog.h"
#include "diffwindow.h"
#include "git/gitlog.h"
#include "git/models/logsmodel.h"
#include "models/historymodel.h"
#include "widgets/graphpainter.h"

#include <QDebug>
#include <QMenu>

HistoryViewWidget::HistoryViewWidget(QWidget *parent) :
      WidgetBase(parent)
{
    setupUi(this);
    _historyModel = new Git::LogsModel(Git::Manager::instance(), this);
//        Git::Manager::instance()->logsCache();
    treeViewHistory->setModel(_historyModel);

    _graphPainter = new GraphPainter(_historyModel, this);
    treeViewHistory->setItemDelegateForColumn(0, _graphPainter);

    connect(Git::Manager::instance(),
            &Git::Manager::pathChanged,
            this,
            &HistoryViewWidget::git_pathChanged);

    _actions = new CommitActions(Git::Manager::instance(), this);
}

HistoryViewWidget::HistoryViewWidget(Git::Manager *git, AppWindow *parent):
      WidgetBase(git, parent)
{
    setupUi(this);
    _historyModel = git->logsModel();
    treeViewHistory->setModel(_historyModel);

    _graphPainter = new GraphPainter(_historyModel, this);
    treeViewHistory->setItemDelegateForColumn(0, _graphPainter);

    connect(Git::Manager::instance(),
            &Git::Manager::pathChanged,
            this,
            &HistoryViewWidget::git_pathChanged);

    _actions = new CommitActions(git, this);
}

void HistoryViewWidget::setBranch(const QString &branchName)
{
    treeViewHistory->setItemDelegateForColumn(0, nullptr);
    _historyModel->setBranch(branchName);
    if (_historyModel->rowCount(QModelIndex()))
        treeViewHistory->setCurrentIndex(_historyModel->index(0));
}

void HistoryViewWidget::saveState(QSettings &settings) const
{
    save(settings, splitter);
}

void HistoryViewWidget::restoreState(QSettings &settings)
{
    restore(settings, splitter);
}

void HistoryViewWidget::on_treeViewHistory_itemActivated(const QModelIndex &index)
{
    auto log = _historyModel->fromIndex(index);
    if (!log)
        return;

    textBrowser->setLog(log);
}

void HistoryViewWidget::on_textBrowser_hashClicked(const QString &hash)
{
    auto index = _historyModel->findIndexByHash(hash);
    if (index.isValid()) {
        treeViewHistory->setCurrentIndex(index);
        on_treeViewHistory_itemActivated(index);
    }
}

void HistoryViewWidget::on_textBrowser_fileClicked(const QString &file)
{
    auto log = textBrowser->log();

    Git::File oldFile;
    Git::File newFile(log->commitHash(), file);
    if (!log->parents().empty()) {
        oldFile = {log->parents().first(), file};
    }
    auto diffWin = new DiffWindow(oldFile, newFile);
    diffWin->showModal();
}

void HistoryViewWidget::on_treeViewHistory_customContextMenuRequested(const QPoint &pos)
{
    Q_UNUSED(pos)
    auto log = _historyModel->fromIndex(treeViewHistory->currentIndex());
    if (!log)
        return;
    _actions->setCommitHash(log->commitHash());

    _actions->popup();
}

void HistoryViewWidget::git_pathChanged()
{
//    _historyModel->reload();
//    if (_historyModel->rowCount(QModelIndex()))
//        treeViewHistory->setCurrentIndex(_historyModel->index(0));
}

