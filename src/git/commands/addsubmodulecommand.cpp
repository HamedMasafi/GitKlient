#include "addsubmodulecommand.h"

namespace Git {

AddSubmoduleCommand::AddSubmoduleCommand(Manager *git)
    : AbstractCommand{git}
{

}

QStringList AddSubmoduleCommand::generateArgs() const
{
    QStringList args{"submodule", "add", _url, _localPath};

    if (!_branch.isEmpty())
        args << "--branch=" + _branch;

    if (_force)
        args << "--force";

    return args;
}

bool AddSubmoduleCommand::force() const
{
    return _force;
}

void AddSubmoduleCommand::setForce(bool newForce)
{
    _force = newForce;
}

QString AddSubmoduleCommand::branch() const
{
    return _branch;
}

void AddSubmoduleCommand::setbranch(QString newbranch)
{
    _branch = newbranch;
}

const QString &AddSubmoduleCommand::url() const
{
    return _url;
}

void AddSubmoduleCommand::setUrl(const QString &newUrl)
{
    _url = newUrl;
}

const QString &AddSubmoduleCommand::localPath() const
{
    return _localPath;
}

void AddSubmoduleCommand::setLocalPath(const QString &newLocalPath)
{
    _localPath = newLocalPath;
}

} // namespace Git
