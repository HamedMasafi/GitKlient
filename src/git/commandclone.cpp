#include "commandclone.h"

#include <QDebug>

namespace Git {

CloneCommand::CloneCommand(QObject *parent) : AbstractCommand(parent) {}

const QString &CloneCommand::repoUrl() const
{
    return _repoUrl;
}

void CloneCommand::setRepoUrl(const QString &newRepoUrl)
{
    _repoUrl = newRepoUrl;
}

const QString &CloneCommand::localPath() const
{
    return _localPath;
}

void CloneCommand::setLocalPath(const QString &newLocalPath)
{
    _localPath = newLocalPath;
}

const QString &CloneCommand::branch() const
{
    return _branch;
}

void CloneCommand::setBranch(const QString &newBranch)
{
    _branch = newBranch;
}

int CloneCommand::depth() const
{
    return _depth;
}

void CloneCommand::setDepth(int newDepth)
{
    _depth = newDepth;
}

const QString &CloneCommand::origin() const
{
    return _origin;
}

void CloneCommand::setOrigin(const QString &newOrigin)
{
    _origin = newOrigin;
}

bool CloneCommand::recursive() const
{
    return _recursive;
}

void CloneCommand::setRecursive(bool newRecursive)
{
    _recursive = newRecursive;
}

QStringList CloneCommand::generateArgs() const
{
    QStringList args{"clone", "--progress", _repoUrl, _localPath};

    if (!_branch.isEmpty())
        args << "--branch=" + _branch;

    if (_depth != -1)
        args << "--depth" << QString::number(_depth);

    if (!_origin.isEmpty())
        args << "--origin=" << _origin;

    if (_recursive)
        args << "--recursive";

    return args;

}

void CloneCommand::parseOutput(const QByteArray &output, const QByteArray &errorOutput)
{
    Q_UNUSED(output)
    auto p = errorOutput.mid(0, errorOutput.lastIndexOf("%"));
    p  = p.mid(p.lastIndexOf(" ") + 1);
    setProgress(p.toDouble());
}

bool CloneCommand::supportProgress() const
{
    return true;
}

} // namespace Git
