#ifndef COMMITACTIONS_H
#define COMMITACTIONS_H

#include "abstractactions.h"

class CommitActions : public AbstractActions
{
    Q_OBJECT

    QString _commitHash;

    DEFINE_ACTION(actionBrowse)
    DEFINE_ACTION(actionCheckout)
    DEFINE_ACTION(actionDiff)

public:
    explicit CommitActions(Git::Manager *git, QWidget *parent = nullptr);

    const QString &commitHash() const;
    void setCommitHash(const QString &newCommitHash);

private slots:
    void browse();
    void checkout();
    void diff();
};

#endif // COMMITACTIONS_H
