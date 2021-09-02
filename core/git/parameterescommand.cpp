#include "parameterescommand.h"

namespace Git {

ParameteresCommand::ParameteresCommand() : AbstractCommand() {}

const QStringList &ParameteresCommand::args() const
{
    return _args;
}

void ParameteresCommand::setArgs(const QStringList &newArgs)
{
    _args = newArgs;
}

void ParameteresCommand::appendArg(const QString &arg)
{
    _args.append(arg);
}

QStringList ParameteresCommand::generateArgs() const
{
    return _args;
}

} // namespace Git
