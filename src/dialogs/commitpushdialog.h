#ifndef COMMITPUSHDIALOG_H
#define COMMITPUSHDIALOG_H

#include "../core/appdialog.h"
#include "ui_commitpushdialog.h"

namespace Git {
class Manager;
}
class ChangedFileActions;
class CommitPushDialog : public AppDialog, private Ui::CommitPushDialog
{
    Q_OBJECT
    ChangedFileActions *_actions;
    Git::Manager *_git;

public:
    explicit CommitPushDialog(Git::Manager *git, QWidget *parent = nullptr);

private Q_SLOTS:
    void checkButtonsEnable();
    void on_pushButtonCommit_clicked();
    void on_pushButtonPush_clicked();
    void on_toolButtonAddAll_clicked();
    void on_toolButtonAddNone_clicked();
    void on_toolButtonAddIndexed_clicked();
    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);
    void on_listWidget_itemClicked(QListWidgetItem *item);
    void on_groupBoxMakeCommit_toggled(bool);
    void on_listWidget_customContextMenuRequested(const QPoint &pos);

private:
    void addFiles();
};

#endif // COMMITPUSHDIALOG_H
