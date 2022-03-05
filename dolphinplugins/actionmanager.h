#ifndef ACTIONMANAGER_H
#define ACTIONMANAGER_H

#include <KIOWidgets/KAbstractFileItemActionPlugin>
#include "statuscache.h"

class ActionManager : public KAbstractFileItemActionPlugin
{
    Q_OBJECT
    StatusCache _cache;

    QAction *_pullAction;
    QAction *_pushAction;
    QAction *_diffAction;
    QAction *_logAction;
    QAction *_openAppAction;
public:
    explicit ActionManager(QObject *parent, const QList<QVariant>&);

    void addMenu(QMenu *menu, const QString &title, const QStringList &args);
    QList<QAction*> actions(const KFileItemListProperties& fileItemInfos, QWidget* parentWidget) override;

private:
    void initActions();
};


#endif // ACTIONMANAGER_H
