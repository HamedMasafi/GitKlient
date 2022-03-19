#include "stash.h"
#include "gitmanager.h"

namespace Git {

const QString &Stash::name() const
{
    return _name;
}

const QString &Stash::authorName() const
{
    return _authorName;
}

const QString &Stash::authorEmail() const
{
    return _authorEmail;
}

const QString &Stash::subject() const
{
    return _subject;
}

const QString &Stash::branch() const
{
    return _branch;
}

const QDateTime &Stash::pushTime() const
{
    return _pushTime;
}

Stash::Stash(Manager *git, const QString &name) : _git(git), _name(name) {}

void Stash::apply()
{
    _git->runGit({"stash", "apply", _name});
}

void Stash::drop()
{
    _git->runGit({"stash", "drop", _name});
}

void Stash::pop()
{
    _git->runGit({"stash", "pop", _name});
}

} // namespace Git
