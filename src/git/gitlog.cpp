#include "gitlog.h"

#include <QDebug>
#include <utility>

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

Log::Log() = default;

Log::Log(QString authorName,
               QString authorEmail,
               QDateTime authDate,
               QString committerName,
               QString committerEmail,
               QDateTime commitDate,
               QString message,
               QString subject,
               QString body,
               QString commitHash,
               QStringList parentHash)
    : _authorName(std::move(authorName)), _authorEmail(std::move(authorEmail)), _authDate(std::move(authDate)),
      _committerName(std::move(committerName)), _committerEmail(std::move(committerEmail)), _commitDate(std::move(commitDate)),
      _message(std::move(message)), _subject(std::move(subject)), _body(std::move(body)), _commitHash(std::move(commitHash)),
      _parentHash(std::move(parentHash))
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

const QStringList &Log::parents() const
{
    return _parentHash;
}

}
