#include "remoteinfodialog.h"
#include "remoteswidget.h"
#include "git/gitmanager.h"
#include "git/models/remotescache.h"

RemotesWidget::RemotesWidget(QWidget *parent) : WidgetBase(parent)
{
    setupUi(this);
    _model = Git::Manager::instance()->remotesModel();
    listView->setModelColumn(0);
    listView->setModel(_model);
}

RemotesWidget::RemotesWidget(Git::Manager *git, GitKlientWindow *parent) :
      WidgetBase(git, parent)
{
    setupUi(this);

    _model = git->remotesModel();
    listView->setModelColumn(0);
    listView->setModel(_model);
}

void RemotesWidget::saveState(QSettings &settings) const
{
    save(settings, splitter);
    save(settings, treeWidget);
}

void RemotesWidget::restoreState(QSettings &settings)
{
    restore(settings, splitter);
    restore(settings, treeWidget);
}

void RemotesWidget::reload()
{
//    listWidget->clear();
//    listWidget->addItems(git()->remotes());
}

void RemotesWidget::on_toolButtonAdd_clicked()
{
    RemoteInfoDialog d{this};
    if (d.exec() == QDialog::Accepted) {
        git()->addRemote(d.remoteName(), d.remoteUrl());
        reload();
    }
}

void RemotesWidget::on_listView_activated(const QModelIndex &index)
{
    auto remote = _model->fromIndex(index);
    if (!remote)
        return;

    labelRemoteName->setText(remote->name);
    labelFetchUrl->setText(remote->fetchUrl);
    labelPushUrl->setText(remote->pushUrl);
    labelDefaultBranch->setText(remote->headBranch);
    treeWidget->clear();

    for (const auto &rb: qAsConst(remote->branches)) {
        auto item = new QTreeWidgetItem(treeWidget);

        item->setText(0, rb.name);
        item->setText(1, rb.remotePushBranch);
        item->setText(2, rb.remotePullBranch);
        item->setText(3, rb.statusText());

        treeWidget->addTopLevelItem(item);
    }
}


//void RemotesWidget::on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
//{
//    Q_UNUSED(previous)

//    toolButtonRemove->setEnabled(true);

//    auto info = git()->remoteDetails(current->text());

//    labelRemoteName->setText(info.name);
//    labelFetchUrl->setText(info.fetchUrl);
//    labelPushUrl->setText(info.pushUrl);
//    labelDefaultBranch->setText(info.headBranch);
//    treeWidget->clear();

//    for (auto &rb: info.branches) {
//        auto item = new QTreeWidgetItem(treeWidget);

//        item->setText(0, rb.name);
//        item->setText(1, rb.remotePushBranch);
//        item->setText(2, rb.remotePullBranch);
//        item->setText(3, rb.statusText());

//        treeWidget->addTopLevelItem(item);
//    }
//}


//void RemotesWidget::on_toolButtonRemove_clicked()
//{
//    if (!listWidget->currentIndex().isValid())
//        return;

//    git()->removeRemote(listWidget->currentItem()->text());
//    reload();
//    toolButtonRemove->setEnabled(false);
//}

