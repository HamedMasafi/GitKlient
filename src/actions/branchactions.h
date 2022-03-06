#ifndef BRANCHACTIONS_H
#define BRANCHACTIONS_H

#include <QObject>

#include "abstractactions.h"

class BranchActions : public AbstractActions
{
    Q_OBJECT

    QString _branchName;
    QString _otherBranch;

    DEFINE_ACTION(actionBrowse)
    DEFINE_ACTION(actionCheckout)
    DEFINE_ACTION(actionDiff)

public:
    BranchActions(Git::Manager *git, QWidget *parent = nullptr);

    const QString &branchName() const;
    void setBranchName(const QString &newBranchName);

    const QString &otherBranch() const;
    void setOtherBranch(const QString &newOtherBranch);

private slots:
    void browse();
    void checkout();
    void diff();
};

#endif // BRANCHACTIONS_H
