#include "changedfilesdialog.h"
#include "diffwindow.h"
#include "git/gitmanager.h"
#include "git/gitfile.h"
#include "commitpushdialog.h"

#include <QDebug>

ChangedFilesDialog::ChangedFilesDialog(QWidget *parent) :
      QDialog(parent)
{
    setupUi(this);
    reload();
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
    Git::File original{Git::Manager::instance()->currentBranch(), item->text()};
    Git::File changed{Git::Manager::instance()->path() + "/" + item->text()};

    auto diffWin = new DiffWindow(original, changed);
    diffWin->setWindowModality(Qt::ApplicationModal);
    diffWin->setAttribute(Qt::WA_DeleteOnClose, true);
    diffWin->show();
}
