#ifndef MERGEDIALOG_H
#define MERGEDIALOG_H

#include "../core/appdialog.h"
#include "ui_mergedialog.h"

namespace Git {
class CommandMerge;
class Manager;
}
class MergeDialog : public AppDialog, private Ui::MergeDialog
{
    Q_OBJECT
public:
    explicit MergeDialog(Git::Manager *git, QWidget *parent = nullptr);
    explicit MergeDialog(Git::Manager *git, const QString &sourceBranch, QWidget *parent = nullptr);
    Git::CommandMerge *command() const;

private:
    QString _sourceBranch;
    void init(Git::Manager *git);
};

#endif // MERGEDIALOG_H
