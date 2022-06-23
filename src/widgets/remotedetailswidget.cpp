#include "remotedetailswidget.h"
#include "git/gitmanager.h"
#include "git/gitremote.h"
#include "git/models/remotesmodel.h"

#include <QDebug>

Git::Manager *RemoteDetailsWidget::git() const
{
    return _git;
}

void RemoteDetailsWidget::setGit(Git::Manager *newGit)
{
    _git = newGit;
//    listWidget->clear();
    listView->setModel(_git->remotesModel());
}

void RemoteDetailsWidget::reload()
{
//    listWidget->clear();
//    listWidget->addItems(_git->remotes());
}

//void RemoteDetailsWidget::on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
//{
//    Q_UNUSED(previous)
//    setBranch(current->text());
//}

RemoteDetailsWidget::RemoteDetailsWidget(QWidget *parent) :
      QWidget(parent)
{
    setupUi(this);

    _git = Git::Manager::instance();
    connect(_git, &Git::Manager::pathChanged, this, &RemoteDetailsWidget::reload);

}

const QString &RemoteDetailsWidget::branch() const
{
    return _branch;
}

void RemoteDetailsWidget::setBranch(const QString &newBranch)
{
    _branch = newBranch;

    if (!_git)
        _git = Git::Manager::instance();
    auto info = _git->remoteDetails(newBranch);

    labelRemoteName->setText(info.name);
    labelFetchUrl->setText(info.fetchUrl);
    labelPushUrl->setText(info.pushUrl);
    labelDefaultBranch->setText(info.headBranch);

    for (auto &rb: info.branches) {
        auto item = new QTreeWidgetItem(treeWidget);

        item->setText(0, rb.name);
        item->setText(1, rb.remotePushBranch);
        item->setText(2, rb.remotePullBranch);
        item->setText(3, rb.statusText());

        treeWidget->addTopLevelItem(item);
    }
}

