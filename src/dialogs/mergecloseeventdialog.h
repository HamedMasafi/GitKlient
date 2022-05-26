#ifndef CLOSEEVENTDIALOG_H
#define CLOSEEVENTDIALOG_H

#include "../core/appdialog.h"
#include "ui_mergecloseeventdialog.h"

class MergeCloseEventDialog : public AppDialog, private Ui::CloseEventDialog
{
    Q_OBJECT

public:
    enum ReturnType {
        LeaveAsIs = QDialog::Rejected,
        MarkAsResolved = QDialog::Accepted,
        DontExit
    };
    explicit MergeCloseEventDialog(QWidget *parent = nullptr);
private slots:
    void on_commandLinkButtonMarkResolved_clicked();
    void on_commandLinkButtonLeaveAsIs_clicked();
    void on_commandLinkButtonDontExit_clicked();
};

#endif // CLOSEEVENTDIALOG_H
