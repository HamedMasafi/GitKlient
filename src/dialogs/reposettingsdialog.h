#ifndef REPOSETTINGSDIALOG_H
#define REPOSETTINGSDIALOG_H

#include "../core/appdialog.h"
#include "ui_reposettingsdialog.h"

namespace Git {
class Manager;
}

class RepoSettingsDialog : public AppDialog, private Ui::RepoSettingsDialog
{
    Q_OBJECT

    Git::Manager *_git;

public:
    explicit RepoSettingsDialog(Git::Manager *git, QWidget *parent = nullptr);

private slots:
    void on_buttonBox_accepted();
};

#endif // REPOSETTINGSDIALOG_H
