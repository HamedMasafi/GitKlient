#ifndef PULLDIALOG_H
#define PULLDIALOG_H

#include "../core/dialog.h"
#include "ui_pulldialog.h"

namespace Git {
class Manager;
}
class PullDialog : public Dialog, private Ui::PullDialog
{
    Q_OBJECT

public:
    explicit PullDialog(QWidget *parent = nullptr, Git::Manager *git = nullptr);

private slots:
    void on_buttonBox_accepted();
};

#endif // PULLDIALOG_H
