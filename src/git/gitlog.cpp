#include "gitlog.h"

#include <QDebug>

namespace Git {

const QString &Log::refLog() const
{
    return _refLog;
}

const QString &Log::branch() const
{
    return _branch;
}

const QString &Log::extraData() const
{
    return _extraData;
}

Log::CommitType Log::type() const
{
    return _type;
}

const QVector<GraphLane> &Log::lanes() const
{
    return _lanes;
}

const QStringList &Log::childs() const
{
    return _childs;
}

const QString &Log::commitShortHash() const
{
    return _commitShortHash;
}

Log::Log() {}

Log::Log(const QString &authorName,
               const QString &authorEmail,
               const QDateTime &authDate,
               const QString &committerName,
               const QString &committerEmail,
               const QDateTime &commitDate,
               const QString &message,
               const QString &subject,
               const QString &body,
               const QString &commitHash,
               const QStringList &parentHash)
    : _authorName(authorName), _authorEmail(authorEmail), _authDate(authDate),
      _committerName(committerName), _committerEmail(committerEmail), _commitDate(commitDate),
      _message(message), _subject(subject), _body(body), _commitHash(commitHash),
      _parentHash(parentHash)
{}

Log::~Log()
{
    qDebug() << "~";
}

const QString &Log::authorName() const
{
    return _authorName;
}

const QString &Log::authorEmail() const
{
    return _authorEmail;
}

const QDateTime &Log::authDate() const
{
    return _authDate;
}

const QString &Log::committerName() const
{
    return _committerName;
}

const QString &Log::committerEmail() const
{
    return _committerEmail;
}

const QDateTime &Log::commitDate() const
{
    return _commitDate;
}

const QString &Log::message() const
{
    return _message;
}

const QString &Log::subject() const
{
    return _subject;
}

const QString &Log::body() const
{
    return _body;
}

const QString &Log::commitHash() const
{
    return _commitHash;
}

const QStringList &Log::parentHash() const
{
    return _parentHash;
}

}
