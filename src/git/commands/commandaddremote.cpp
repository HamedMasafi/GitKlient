#include "commandaddremote.h"

namespace Git {

CommandAddRemote::CommandAddRemote(QObject *parent) : AbstractCommand{parent} {}

QStringList CommandAddRemote::generateArgs() const
{
    QStringList args{"remote", "add"};

    if (_fetch)
        args << "--fetch";
    if (_mirror)
        args << "--mirrot";
    appendBool(_tags, args, "tags");

    if (_master != QString())
        args << "--master=" + _master;

    args << _remoteName << _url;
    return args;
}

const QString &CommandAddRemote::remoteName() const
{
    return _remoteName;
}

void CommandAddRemote::setRemoteName(const QString &newRemoteName)
{
    _remoteName = newRemoteName;
}

OptionalBool CommandAddRemote::tags() const
{
    return _tags;
}

void CommandAddRemote::setTags(OptionalBool newTags)
{
    _tags = newTags;
}

void CommandAddRemote::setTags(Qt::CheckState newTags)
{
    setTags(checkStateToOptionalBool(newTags));
}

bool CommandAddRemote::mirror() const
{
    return _mirror;
}

void CommandAddRemote::setMirror(bool newMirror)
{
    _mirror = newMirror;
}

const QString &CommandAddRemote::master() const
{
    return _master;
}

void CommandAddRemote::setMaster(const QString &newMaster)
{
    _master = newMaster;
}

bool CommandAddRemote::fetch() const
{
    return _fetch;
}

void CommandAddRemote::setFetch(bool newFetch)
{
    _fetch = newFetch;
}

const QString &CommandAddRemote::url() const
{
    return _url;
}

void CommandAddRemote::setUrl(const QString &newUrl)
{
    _url = newUrl;
}

} // namespace Git
