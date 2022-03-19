#ifndef SELECTBRANCHESTODIFFDIALOG_H
#define SELECTBRANCHESTODIFFDIALOG_H

#include "../core/dialog.h"
#include "ui_selectbranchestodiffdialog.h"

namespace Git {
class Manager;
}

class SelectBranchesToDiffDialog : public Dialog, private Ui::SelectBranchesToDiffDialog
{
    Q_OBJECT

public:
    explicit SelectBranchesToDiffDialog(Git::Manager *git, QWidget *parent = nullptr);

    QString oldBranch() const;
    QString newBranch() const;

private slots:
    void on_buttonBox_accepted();
};

#endif // SELECTBRANCHESTODIFFDIALOG_H
