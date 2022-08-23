#include "commandpull.h"

#ifdef GIT_GUI
#include "ui_commandpullwidget.h"
#include <KMessageBox>
#include <KLocalizedString>
#endif

#include <QDebug>
#include <QLabel>

namespace Git {

bool CommandPull::squash() const
{
    return _squash;
}

void CommandPull::setSquash(bool newSquash)
{
    _squash = newSquash;
}

bool CommandPull::noFf() const
{
    return _noFf;
}

void CommandPull::setNoFf(bool newNoFf)
{
    _noFf = newNoFf;
}

bool CommandPull::ffOnly() const
{
    return _ffOnly;
}

void CommandPull::setFfOnly(bool newFfOnly)
{
    _ffOnly = newFfOnly;
}

bool CommandPull::noCommit() const
{
    return _noCommit;
}

void CommandPull::setNoCommit(bool newNoCommit)
{
    _noCommit = newNoCommit;
}

bool CommandPull::prune() const
{
    return _prune;
}

void CommandPull::setPrune(bool newPrune)
{
    _prune = newPrune;
}

bool CommandPull::tags() const
{
    return _tags;
}

void CommandPull::setTags(bool newTags)
{
    _tags = newTags;
}

void CommandPull::parseOutput(const QByteArray &output, const QByteArray &errorOutput)
{
    Q_UNUSED(errorOutput)
#ifdef GIT_GUI
    if (output.contains("Already up to date.")) {
        _ui->labelStatus->setText("Already up to date.");
    }
    if (errorOutput.startsWith("fatal:")) {
        _ui->labelStatus->setText(errorOutput.mid(6));
        KMessageBox::error(_widget, errorOutput.mid(6), i18n("Error"));
    }
#endif
}

bool CommandPull::supportWidget() const
{
    return true;
}

QWidget *CommandPull::createWidget()
{
#ifdef GIT_GUI
    _widget = new QWidget;
    _ui = new Ui::CommandPullWidget;
    _ui->setupUi(_widget);
    return _widget;
#else
    return nullptr;
#endif
}

const QString &CommandPull::remote() const
{
    return _remote;
}

void CommandPull::setRemote(const QString &newRemote)
{
    _remote = newRemote;
}

const QString &CommandPull::branch() const
{
    return _branch;
}

void CommandPull::setBranch(const QString &newBranch)
{
    _branch = newBranch;
}

QStringList CommandPull::generateArgs() const
{
    QStringList args{"pull", _remote, _branch};
    if (_squash)
        args.append("--squash");
    if (_noFf)
        args.append("--no-ff");
    if (_ffOnly)
        args.append("--ff-only");
    if (_noCommit)
        args.append("--no-commit");
    if (_prune)
        args.append("--prune");
    if (_tags)
        args.append("--tags");
    return args;
}

CommandPull::CommandPull() = default;

CommandPull::~CommandPull()
{
#ifdef GIT_GUI
    if (_widget)
        _widget->deleteLater();

    delete _ui;
#endif
}

} // namespace Git
