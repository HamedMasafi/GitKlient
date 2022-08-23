#ifndef STASHACTIONS_H
#define STASHACTIONS_H

#include <QObject>

#include "abstractactions.h"

class StashActions : public AbstractActions
{
    Q_OBJECT

public:
    explicit StashActions(Git::Manager *git, QWidget *parent = nullptr);

    const QString &stashName() const;
    void setStashName(const QString &newStashName);

public slots:
    void apply();
    void drop();
    void pop();
    void diff();
    void create();

private:
    QString _stashName;
    DEFINE_ACTION(actionPop)
    DEFINE_ACTION(actionApply)
    DEFINE_ACTION(actionDrop)
    DEFINE_ACTION(actionDiff)
    DEFINE_ACTION(actionNew)
};

#endif // STASHACTIONS_H
