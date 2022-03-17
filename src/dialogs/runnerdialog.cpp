#include "runnerdialog.h"

#include "git/gitmanager.h"

#include <KLocalizedString>

#include <QProcess>
#include <QDebug>

RunnerDialog::RunnerDialog(QWidget *parent) :
      QDialog(parent)
{
    setupUi(this);

    _git = new QProcess{this};
    _git->setProgram("git");
    _git->setWorkingDirectory(Git::Manager::instance()->path());

    connect(_git,
            &QProcess::readyReadStandardOutput,
            this,
            &RunnerDialog::git_readyReadStandardOutput);
    connect(_git,
            &QProcess::readyReadStandardError,
            this,
            &RunnerDialog::git_readyReadStandardError);
}

void RunnerDialog::run(const QStringList &args)
{
    _mode = RunByArgs;
    lineEditCommand->setText("git " + args.join(" "));
    _git->setArguments(args);
    _git->start();
}

void RunnerDialog::run(Git::AbstractCommand *command)
{
    _mode = RunByCommand;
    if (command->supportWidget()) {
        auto w = command->createWidget();
        tabWidget->insertTab(0, w, i18n("View"));
        tabWidget->setCurrentIndex(0);
    }

    auto args = command->generateArgs();
    lineEditCommand->setText("git " + args.join(" "));

    if (command->supportProgress()) {
        progressBar->show();
        connect(command, &Git::AbstractCommand::progressChanged, progressBar, &QProgressBar::setValue);
    } else {
        progressBar->hide();
    }
    _git->setArguments(args);
    _git->start();
    _cmd = command;
}

void RunnerDialog::git_readyReadStandardOutput()
{
    auto buffer = _git->readAllStandardOutput();
    qDebug() << "OUT" << buffer;
//    textBrowser->setTextColor(Qt::black);
    textBrowser->append(buffer);

    if (_cmd)
        _cmd->parseOutput(buffer, QByteArray());
}

void RunnerDialog::git_readyReadStandardError()
{
    auto buffer = _git->readAllStandardError();
    qDebug() << "ERROR" << buffer;
//    textBrowser->setTextColor(Qt::red);
    textBrowser->append(buffer);

    if (_cmd)
        _cmd->parseOutput(QByteArray(), buffer);
}
