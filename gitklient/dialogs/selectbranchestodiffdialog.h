#ifndef SELECTBRANCHESTODIFFDIALOG_H
#define SELECTBRANCHESTODIFFDIALOG_H

#include "ui_selectbranchestodiffdialog.h"

namespace Git {
class Manager;
}

class SelectBranchesToDiffDialog : public QDialog, private Ui::SelectBranchesToDiffDialog
{
    Q_OBJECT

public:
    explicit SelectBranchesToDiffDialog(Git::Manager *git, QWidget *parent = nullptr);

    QString oldBranch() const;
    QString newBranch() const;
};

#endif // SELECTBRANCHESTODIFFDIALOG_H
