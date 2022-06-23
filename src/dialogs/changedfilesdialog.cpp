#include "changedfilesdialog.h"
#include "actions/changedfileactions.h"
#include "commitpushdialog.h"
#include "diffwindow.h"
#include "git/gitfile.h"
#include "git/gitmanager.h"
#include "GitKlientSettings.h"
#include <QDebug>

ChangedFilesDialog::ChangedFilesDialog(Git::Manager *git, QWidget *parent) :
      AppDialog(git, parent)
{
    setupUi(this);
    reload();
    _actions = new ChangedFileActions(git, this);
    connect(_actions, &ChangedFileActions::reloadNeeded, this, &ChangedFilesDialog::reload);
}

void ChangedFilesDialog::on_pushButtonCommitPush_clicked()
{
    CommitPushDialog d(_git, this);
    d.exec();
    reload();
}

void ChangedFilesDialog::reload()
{
    listWidget->clear();
    auto files = _git->changedFiles();

    for (auto i = files.begin(); i != files.end(); ++i) {
        auto item = new QListWidgetItem(listWidget);
        item->setText(i.key());
        switch (i.value()) {
        case Git::Manager::Modified:
            item->setForeground(GitKlientSettings::diffModifiedColor());
            break;

        default:
            break;
        }
        listWidget->addItem(item);
    }
}

void ChangedFilesDialog::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    if (!item)
        return;
    _actions->setFilePath(listWidget->currentItem()->text());
    _actions->diff();
}

void ChangedFilesDialog::on_listWidget_customContextMenuRequested(const QPoint &pos)
{
    Q_UNUSED(pos)
    if (listWidget->currentRow() == -1)
        return;

    _actions->setFilePath(listWidget->currentItem()->text());
    _actions->popup();
}
