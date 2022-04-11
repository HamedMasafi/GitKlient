#ifndef SWITCHBRANCHDIALOG_H
#define SWITCHBRANCHDIALOG_H

#include "ui_switchbranchdialog.h"
#include "core/dialog.h"

class SwitchBranchDialog : public Dialog, private Ui::SwitchBranchDialog
{
    Q_OBJECT

public:
    explicit SwitchBranchDialog(Git::Manager *git, QWidget *parent = nullptr);

private slots:
    void on_buttonBox_accepted();
};

#endif // SWITCHBRANCHDIALOG_H