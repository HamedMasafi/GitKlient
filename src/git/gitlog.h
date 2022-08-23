#ifndef GITLOG_H
#define GITLOG_H

#include <QDateTime>
#include <QString>
#include <QVector>
#include "gitgraphlane.h"

namespace Git {

class Log
{
public:
    enum CommitType {
        NormalCommit,
        InitialCommit,
        ForkCommit,
        MergeCommit
    };

    Log();
    Log(QString authorName,
           QString authorEmail,
           QDateTime authDate,
           QString committerName,
           QString committerEmail,
           QDateTime commitDate,
           QString message,
           QString subject,
           QString body,
           QString commitHash,
           QStringList parentHash);
    ~Log();

    const QString &authorName() const;
    const QString &authorEmail() const;
    const QDateTime &authDate() const;
    const QString &committerName() const;
    const QString &committerEmail() const;
    const QDateTime &commitDate() const;
    const QString &message() const;
    const QString &subject() const;
    const QString &body() const;
    const QString &commitHash() const;
    const QStringList &parents() const;
    const QString &refLog() const;
    const QString &branch() const;
    const QString &extraData() const;
    CommitType type() const;
    const QVector<GraphLane> &lanes() const;

    const QStringList &childs() const;

    const QString &commitShortHash() const;

private:
    QString _authorName;
    QString _authorEmail;
    QDateTime _authDate;
    QString _committerName;
    QString _committerEmail;
    QDateTime _commitDate;
    QString _message;
    QString _subject;
    QString _body;
    QString _commitHash;
    QString _commitShortHash;
    QStringList _parentHash;
    QString _refLog;
    QString _branch;
    QString _extraData;
    CommitType _type;
    QVector<GraphLane> _lanes;
    QStringList _childs;

    friend class LogList;
    friend class GitManager;
    friend class LogsModel;
};

}

#endif // GITLOG_H
