#ifndef REMOTESACTIONS_H
#define REMOTESACTIONS_H

#include "abstractactions.h"

class RemotesActions: public AbstractActions
{
    Q_OBJECT

    DEFINE_ACTION(actionCreate)
    DEFINE_ACTION(actionRemove)

public:
    RemotesActions(Git::Manager *git, QWidget *parent = nullptr);

    const QString &remoteName() const;
    void setRemoteName(const QString &newRemoteName);

private slots:
    void create();
    void remove();

private:
    QString _remoteName;
};

#endif // REMOTESACTIONS_H
