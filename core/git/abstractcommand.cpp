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

AbstractCommand::AbstractCommand() {}

AbstractCommand::AbstractCommand(const QStringList &args) : _args(args)
{}

AbstractCommand::~AbstractCommand()
{
}

void AbstractCommand::parseOutput(const QByteArray &output, const QByteArray &errorOutput)
{
    Q_UNUSED(output);
    Q_UNUSED(errorOutput)
}

AbstractCommand::AbstractCommand(Manager *git) : _git(git)
{}

} // namespace Git
