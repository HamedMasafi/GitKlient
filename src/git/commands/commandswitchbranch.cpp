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
    QStringList cmd{"switch", _target};
    if (_force)
        cmd.append("--force");
    return cmd;
}

bool CommandSwitchBranch::force() const
{
    return _force;
}

void CommandSwitchBranch::setForce(bool newForce)
{
    _force = newForce;
}

} // namespace Git
