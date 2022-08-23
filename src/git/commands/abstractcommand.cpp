#include "abstractcommand.h"

#include <utility>

namespace Git {

bool AbstractCommand::isValid() const
{
    return _isValid;
}

QWidget *AbstractCommand::createWidget()
{
    return nullptr;
}

int AbstractCommand::progress() const
{
    return m_progress;
}

AbstractCommand::Status AbstractCommand::status() const
{
    return _status;
}

void AbstractCommand::setStatus(Status newStatus)
{
    _status = newStatus;
}

const QString &AbstractCommand::errorMessage() const
{
    return _errorMessage;
}

void AbstractCommand::setErrorMessage(const QString &newErrorMessage)
{
    _errorMessage = newErrorMessage;
}

AbstractCommand::AbstractCommand(QObject *parent) : QObject(parent) {}

AbstractCommand::AbstractCommand(QStringList args) : QObject(), _args(std::move(args)) {}

AbstractCommand::AbstractCommand(Manager *git) : QObject(), _git(git) {}

AbstractCommand::~AbstractCommand() = default;

void AbstractCommand::parseOutput(const QByteArray &output, const QByteArray &errorOutput)
{
    Q_UNUSED(output)
    Q_UNUSED(errorOutput)
}

void AbstractCommand::setProgress(int newProgress)
{
    if (m_progress == newProgress)
        return;
    m_progress = newProgress;
    emit progressChanged(newProgress);
}

void AbstractCommand::appendBool(OptionalBool b, QStringList &cmd, const QString &name) const
{
    switch (b) {
    case OptionalBool::True:
        cmd.append("--" + name);
        break;
    case OptionalBool::False:
        cmd.append("--no-" + name);
        break;
    case OptionalBool::Unset:
        break;
    }
}

void AbstractCommand::appendBool(bool b, QStringList &cmd, const QString &name) const
{
    if (b)
        cmd.append("--" + name);
    else
        cmd.append("--no-" + name);
}

OptionalBool checkStateToOptionalBool(Qt::CheckState checkState)
{
    switch (checkState) {
    case Qt::Unchecked:
        return OptionalBool::False;
    case Qt::PartiallyChecked:
        return OptionalBool::Unset;
    case Qt::Checked:
        return OptionalBool::True;
    }
    return OptionalBool::Unset;
}


} // namespace Git
