#include "commandfetch.h"

#include <QDebug>
#include <QLabel>

namespace Git {

bool CommandFetch::noFf() const
{
    return _noFf;
}

void CommandFetch::setNoFf(bool newNoFf)
{
    _noFf = newNoFf;
}

bool CommandFetch::ffOnly() const
{
    return _ffOnly;
}

void CommandFetch::setFfOnly(bool newFfOnly)
{
    _ffOnly = newFfOnly;
}

bool CommandFetch::noCommit() const
{
    return _noCommit;
}

void CommandFetch::setNoCommit(bool newNoCommit)
{
    _noCommit = newNoCommit;
}

bool CommandFetch::prune() const
{
    return _prune;
}

void CommandFetch::setPrune(bool newPrune)
{
    _prune = newPrune;
}

bool CommandFetch::tags() const
{
    return _tags;
}

void CommandFetch::setTags(bool newTags)
{
    _tags = newTags;
}

void CommandFetch::parseOutput(const QByteArray &output, const QByteArray &errorOutput)
{
    Q_UNUSED(output)
    Q_UNUSED(errorOutput)
}

const QString &CommandFetch::remote() const
{
    return _remote;
}

void CommandFetch::setRemote(const QString &newRemote)
{
    _remote = newRemote;
}

const QString &CommandFetch::branch() const
{
    return _branch;
}

void CommandFetch::setBranch(const QString &newBranch)
{
    _branch = newBranch;
}

QStringList CommandFetch::generateArgs() const
{
    QStringList args{"fetch", _remote};
    if (!_branch.isEmpty())
        args.append(_branch);
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

CommandFetch::CommandFetch() = default;

} // namespace Git
