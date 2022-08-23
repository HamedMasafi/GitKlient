#include "gitsubmodule.h"

namespace Git {

Submodule::Submodule() = default;

const QString &Submodule::path() const
{
    return _path;
}

void Submodule::setPath(const QString &newPath)
{
    _path = newPath;
}

const QString &Submodule::commitHash() const
{
    return _commitHash;
}

void Submodule::setCommitHash(const QString &newCommitHash)
{
    _commitHash = newCommitHash;
}

const QString &Submodule::refName() const
{
    return _refName;
}

void Submodule::setRefName(const QString &newRefName)
{
    _refName = newRefName;
}

} // namespace Git
