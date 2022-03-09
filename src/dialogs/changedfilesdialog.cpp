#include "changedfilesdialog.h"
#include "actions/changedfileactions.h"
#include "commitpushdialog.h"
#include "diffwindow.h"
#include "git/gitfile.h"
#include "git/gitmanager.h"

#include <QDebug>

ChangedFilesDialog::ChangedFilesDialog(QWidget *parent) :
      QDialog(parent)
{
    setupUi(this);
    reload();
    _actions = new ChangedFileActions(Git::Manager::instance(), this);
}

void ChangedFilesDialog::on_pushButtonCommitPush_clicked()
{
    CommitPushDialog d(this);
    d.exec();
    reload();
}

void ChangedFilesDialog::reload()
{
    listWidget->clear();
    auto files = Git::Manager::instance()->changedFiles();

    for (auto i = files.begin(); i != files.end(); ++i) {
        auto item = new QListWidgetItem(listWidget);
        item->setText(i.key());
        switch (i.value()) {
        case Git::Manager::Modified:
            item->setForeground(Qt::blue);
            break;

        default:
            break;
        }
        listWidget->addItem(item);
    }
}

void ChangedFilesDialog::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    _actions->setFilePath(listWidget->currentItem()->text());
    _actions->diff();

    /*Git::File original{Git::Manager::instance()->currentBranch(), item->text()};
    Git::File changed{Git::Manager::instance()->path() + "/" + item->text()};

    auto diffWin = new DiffWindow(original, changed);
    diffWin->setWindowModality(Qt::ApplicationModal);
    diffWin->setAttribute(Qt::WA_DeleteOnClose, true);
    diffWin->show();*/
}

void ChangedFilesDialog::on_listWidget_customContextMenuRequested(const QPoint &pos)
{
    if (listWidget->currentRow() == -1)
        return;

    _actions->setFilePath(listWidget->currentItem()->text());
    _actions->popup(listWidget->mapToGlobal(pos));
}
