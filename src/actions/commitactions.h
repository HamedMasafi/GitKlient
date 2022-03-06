#ifndef COMMITACTIONS_H
#define COMMITACTIONS_H

#include "abstractactions.h"

class CommitActions : public AbstractActions
{
    Q_OBJECT

    QString _commitHash;

    DEFINE_ACTION(actionBrowse)

public:
    CommitActions(Git::Manager *git, QWidget *parent = nullptr);

    const QString &commitHash() const;
    void setCommitHash(const QString &newCommitHash);

private slots:
    void browse();
};

#endif // COMMITACTIONS_H
