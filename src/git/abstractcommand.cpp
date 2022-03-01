#include "abstractcommand.h"

namespace Git {

bool AbstractCommand::isValid() const
{
    return _isValid;
}

QWidget *AbstractCommand::createWidget() const
{
    return nullptr;
}

int AbstractCommand::progress() const
{
    return m_progress;
}

AbstractCommand::AbstractCommand(QObject *parent) : QObject(parent) {}

AbstractCommand::AbstractCommand(const QStringList &args) : QObject(), _args(args) {}

AbstractCommand::AbstractCommand(Manager *git) : QObject(), _git(git) {}

AbstractCommand::~AbstractCommand()
{
}

void AbstractCommand::parseOutput(const QByteArray &output, const QByteArray &errorOutput)
{
    Q_UNUSED(output);
    Q_UNUSED(errorOutput)
}

void AbstractCommand::setProgress(int newProgress)
{
    if (m_progress == newProgress)
        return;
    m_progress = newProgress;
    emit progressChanged(newProgress);
}


} // namespace Git
