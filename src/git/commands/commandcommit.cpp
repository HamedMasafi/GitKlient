#include "commandcommit.h"

namespace Git {

CommandCommit::CommandCommit() = default;

QStringList CommandCommit::generateArgs() const
{
    QStringList args{"commit", "--message", _message};

    if (_amend)
        args.append("--amend");

    if (_includeStatus)
        args.append("--status");
    else
        args.append("--no-status");

    return args;
}

const QString &CommandCommit::message() const
{
    return _message;
}

void CommandCommit::setMessage(const QString &newMessage)
{
    _message = newMessage;
}

bool CommandCommit::amend() const
{
    return _amend;
}

void CommandCommit::setAmend(bool newAmend)
{
    _amend = newAmend;
}

bool CommandCommit::includeStatus() const
{
    return _includeStatus;
}

void CommandCommit::setIncludeStatus(bool newIncludeStatus)
{
    _includeStatus = newIncludeStatus;
}

} // namespace Git
