#ifndef REMOTESACTIONS_H
#define REMOTESACTIONS_H

#include "abstractactions.h"

class RemotesActions: public AbstractActions
{
    Q_OBJECT

    DEFINE_ACTION(actionCreate)
    DEFINE_ACTION(actionRemove)
    DEFINE_ACTION(actionRename)
    DEFINE_ACTION(actionUpdate)
    DEFINE_ACTION(actionChangeUrl)

public:
    explicit RemotesActions(Git::Manager *git, QWidget *parent = nullptr);

    const QString &remoteName() const;
    void setRemoteName(const QString &newRemoteName);

private slots:
    void create();
    void remove();
    void changeUrl();
    void rename();
    void update();

private:
    QString _remoteName;
};

#endif // REMOTESACTIONS_H
