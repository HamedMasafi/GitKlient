#ifndef CHANGEDFILESDIALOG_H
#define CHANGEDFILESDIALOG_H

#include "ui_changedfilesdialog.h"

class ChangedFileActions;
class ChangedFilesDialog : public QDialog, private Ui::ChangedFilesDialog
{
    Q_OBJECT
    ChangedFileActions *_actions;

public:
    explicit ChangedFilesDialog(QWidget *parent = nullptr);

private Q_SLOTS:
    void on_pushButtonCommitPush_clicked();

    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);
    void on_listWidget_customContextMenuRequested(const QPoint &pos);
private:
    void reload();
};

#endif // CHANGEDFILESDIALOG_H
