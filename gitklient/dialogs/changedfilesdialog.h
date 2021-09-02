#ifndef CHANGEDFILESDIALOG_H
#define CHANGEDFILESDIALOG_H

#include "ui_changedfilesdialog.h"

class ChangedFilesDialog : public QDialog, private Ui::ChangedFilesDialog
{
    Q_OBJECT

public:
    explicit ChangedFilesDialog(QWidget *parent = nullptr);

private Q_SLOTS:
    void on_pushButtonCommitPush_clicked();

    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

private:
    void reload();
};

#endif // CHANGEDFILESDIALOG_H
