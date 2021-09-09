#ifndef KLIENTDOLPHINOVERLAYPLUGIN_H
#define KLIENTDOLPHINOVERLAYPLUGIN_H

#ifdef GK_TEST
#   include <QObject>
#else
#   include <KOverlayIconPlugin>
#endif
#include <QCache>
#include <QMap>
#include "../src/git/filestatus.h"

class KlientDolphinOverlayPlugin : public
#ifdef GK_TEST
                                   QObject
#else
                                   KOverlayIconPlugin
#endif
{
    Q_PLUGIN_METADATA(IID "com.owncloud.ovarlayiconplugin" FILE "klientdolphinoverlayplugin.json")
    Q_OBJECT

    QMap<QString, FileStatus::Status> _statusCache;
    QString _lastDir;

public:

    KlientDolphinOverlayPlugin(QObject *parent = nullptr);

    QStringList getOverlays(const QUrl &url)
#ifdef GK_TEST
        ;
#else
        override;
#endif

};

#endif // KLIENTDOLPHINOVERLAYPLUGIN_H
