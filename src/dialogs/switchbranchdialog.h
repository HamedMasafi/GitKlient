#ifndef SWITCHBRANCHDIALOG_H
#define SWITCHBRANCHDIALOG_H

#include "ui_switchbranchdialog.h"
#include "core/appdialog.h"

namespace Git{
class CommandSwitchBranch;
}
class SwitchBranchDialog : public AppDialog, private Ui::SwitchBranchDialog
{
    Q_OBJECT

public:
    explicit SwitchBranchDialog(Git::Manager *git, QWidget *parent = nullptr);

    Git::CommandSwitchBranch *command() const;

private slots:
//    void on_buttonBox_accepted();
};

#endif // SWITCHBRANCHDIALOG_H
