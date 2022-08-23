#include "gittag.h"

namespace Git {

Tag::Tag() = default;

const QString &Tag::name() const
{
    return _name;
}

void Tag::setName(const QString &newName)
{
    _name = newName;
}

const QString &Tag::message() const
{
    return _message;
}

void Tag::setMessage(const QString &newMessage)
{
    _message = newMessage;
}

const QString &Tag::taggerEmail() const
{
    return _taggerEmail;
}

void Tag::setTaggerEmail(const QString &newTaggerEmail)
{
    _taggerEmail = newTaggerEmail;
}

}
