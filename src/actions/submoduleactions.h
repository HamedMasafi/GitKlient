#ifndef SUBMODULEACTIONS_H
#define SUBMODULEACTIONS_H

#include "abstractactions.h"

class SubmoduleActions: public AbstractActions
{
    Q_OBJECT
        QString _subModuleName;

public:
    SubmoduleActions(Git::Manager *git, QWidget *parent = nullptr);

    void init();
    void update();
    void create();
    void deinit();
    void sync();

    const QString &subModuleName() const;
    void setSubModuleName(const QString &newSubModuleName);

private:
    QString _submoduleName;
    DEFINE_ACTION(actionInit)
    DEFINE_ACTION(actionUpdate)
    DEFINE_ACTION(actionCreate)
    DEFINE_ACTION(actionDeinit)
    DEFINE_ACTION(actionSync)
};

#endif // SUBMODULEACTIONS_H
