#include "commandmerge.h"

namespace Git {

CommandMerge::CommandMerge(Manager *git)
    : AbstractCommand{git}
{

}

QStringList CommandMerge::generateArgs() const
{
    QStringList  cmd{"merge"};
    switch (_ff) {
    case FastForwardType::Unset:
        break;
    case FastForwardType::Yes:
        cmd.append("--ff");
        break;
    case FastForwardType::No:
        cmd.append("--no-ff");
        break;
    case FastForwardType::OnlyFastForward:
        cmd.append("--ff-only");
        break;
    }

    appendBool(_commit, cmd, "commit");
    appendBool(_squash, cmd, "squash");
    appendBool(_allowUnrelatedHistories, cmd, "allow-unrelated-histories");

    if (_strategy != QString())
        cmd.append("--strategy=" + _strategy);

    return cmd;
}

OptionalBool CommandMerge::commit() const
{
    return _commit;
}

void CommandMerge::setCommit(OptionalBool newCommit)
{
    _commit = newCommit;
}

OptionalBool CommandMerge::allowUnrelatedHistories() const
{
    return _allowUnrelatedHistories;
}

void CommandMerge::setAllowUnrelatedHistories(OptionalBool newAllowUnrelatedHistories)
{
    _allowUnrelatedHistories = newAllowUnrelatedHistories;
}

FastForwardType CommandMerge::ff() const
{
    return _ff;
}

void CommandMerge::setFf(FastForwardType newFf)
{
    _ff = newFf;
}

bool CommandMerge::squash() const
{
    return _squash;
}

void CommandMerge::setSquash(bool newSquash)
{
    _squash = newSquash;
}

const QString &CommandMerge::fromBranch() const
{
    return _fromBranch;
}

void CommandMerge::setFromBranch(const QString &newFromBranch)
{
    _fromBranch = newFromBranch;
}

const QString &CommandMerge::strategy() const
{
    return _strategy;
}

void CommandMerge::setStrategy(const QString &newStrategy)
{
    _strategy = newStrategy;
}

} // namespace Git
