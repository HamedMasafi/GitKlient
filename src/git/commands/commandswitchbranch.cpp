#include "commandswitchbranch.h"

namespace Git {

CommandSwitchBranch::CommandSwitchBranch(Manager *git) : AbstractCommand(git)
{

}

const QString &CommandSwitchBranch::target() const
{
    return _target;
}

void CommandSwitchBranch::setTarget(const QString &newTarget)
{
    _target = newTarget;
}

CommandSwitchBranch::Mode CommandSwitchBranch::mode() const
{
    return _mode;
}

void CommandSwitchBranch::setMode(Mode newMode)
{
    _mode = newMode;
}

QStringList CommandSwitchBranch::generateArgs() const
{
    return {"switch", _target};
}

} // namespace Git
