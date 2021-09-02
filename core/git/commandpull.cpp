#include "commandpull.h"

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

QStringList CommandPull::generateArgs() const
{
    QStringList args{"pull"};
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

CommandPull::CommandPull() {}

} // namespace Git
