#ifndef COMMITPUSHDIALOG_H
#define COMMITPUSHDIALOG_H

#include "ui_commitpushdialog.h"

class CommitPushDialog : public QDialog, private Ui::CommitPushDialog
{
    Q_OBJECT

public:
    explicit CommitPushDialog(QWidget *parent = nullptr);

private Q_SLOTS:
    void checkButtonsEnable();
    void on_pushButtonCommit_clicked();
    void on_pushButtonPush_clicked();
    void on_toolButtonAddAll_clicked();
    void on_toolButtonAddNone_clicked();
    void on_toolButtonAddIndexed_clicked();
    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);


private:
    void addFiles();
};

#endif // COMMITPUSHDIALOG_H
