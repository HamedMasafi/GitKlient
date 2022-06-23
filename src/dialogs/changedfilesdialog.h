#ifndef CHANGEDFILESDIALOG_H
#define CHANGEDFILESDIALOG_H

#include "../core/appdialog.h"
#include "ui_changedfilesdialog.h"

class ChangedFileActions;
class ChangedFilesDialog : public AppDialog, private Ui::ChangedFilesDialog
{
    Q_OBJECT
    ChangedFileActions *_actions;

public:
    explicit ChangedFilesDialog(Git::Manager *git, QWidget *parent = nullptr);

private Q_SLOTS:
    void on_pushButtonCommitPush_clicked();

    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);
    void on_listWidget_customContextMenuRequested(const QPoint &pos);
private:
    void reload();
};

#endif // CHANGEDFILESDIALOG_H
