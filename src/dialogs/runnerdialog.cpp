#include "runnerdialog.h"

#include <QDebug>
#include <QProcess>
#include "git/gitmanager.h"

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
    lineEditCommand->setText("git " + args.join(" "));
    _git->setArguments(args);
    _git->start();
}

void RunnerDialog::run(const Git::AbstractCommand &command)
{
    if (command.supportWidget()) {
        auto w = command.createWidget();
        tabWidget->addTab(w, i18n("View"));
    }

    auto args = command.generateArgs();
    lineEditCommand->setText("git " + args.join(" "));
    progressBar->setVisible(command.supportProgress());
    _git->setArguments(args);
    _git->start();
}

void RunnerDialog::git_readyReadStandardOutput()
{
    auto buffer = _git->readAllStandardOutput();
    qDebug() << "OUT" << buffer;
    textBrowser->setTextColor(Qt::black);
    textBrowser->append(buffer);
}

void RunnerDialog::git_readyReadStandardError()
{
    auto buffer = _git->readAllStandardError();
    qDebug() << "ERROR" << buffer;
    textBrowser->setTextColor(Qt::red);
    textBrowser->append(buffer);
}
