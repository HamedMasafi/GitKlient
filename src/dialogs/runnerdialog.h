#ifndef RUNNERDIALOG_H
#define RUNNERDIALOG_H

#include "../core/dialog.h"
#include "ui_runnerdialog.h"

namespace Git {
class AbstractCommand;
}
class QProcess;
class RunnerDialog : public Dialog, private Ui::RunnerDialog
{
    Q_OBJECT

    QProcess *_git;

    enum Mode {
        None,
        RunByArgs,
        RunByCommand
    };
    Mode _mode{None};
    Git::AbstractCommand *_cmd{nullptr};

public:
    explicit RunnerDialog(QWidget *parent = nullptr);
    void run(const QStringList &args);
    void run(Git::AbstractCommand *command);

private slots:
    void git_readyReadStandardOutput();
    void git_readyReadStandardError();
};

#endif // RUNNERDIALOG_H
