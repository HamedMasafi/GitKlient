#ifndef RUNNERDIALOG_H
#define RUNNERDIALOG_H

#include "ui_runnerdialog.h"

namespace Git {
class AbstractCommand;
}
class QProcess;
class RunnerDialog : public QDialog, private Ui::RunnerDialog
{
    Q_OBJECT

    QProcess *_git;

public:
    explicit RunnerDialog(QWidget *parent = nullptr);
    void run(const QStringList &args);
    void run(const Git::AbstractCommand &command);

private slots:
    void git_readyReadStandardOutput();
    void git_readyReadStandardError();
};

#endif // RUNNERDIALOG_H
