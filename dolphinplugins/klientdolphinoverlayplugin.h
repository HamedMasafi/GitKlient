#ifndef KLIENTDOLPHINOVERLAYPLUGIN_H
#define KLIENTDOLPHINOVERLAYPLUGIN_H

#include "../src/git/filestatus.h"
#include "statuscache.h"
#include <KOverlayIconPlugin>
#include <QCache>
#include <QMap>

class KlientDolphinOverlayPlugin : public KOverlayIconPlugin
{
    Q_PLUGIN_METADATA(IID "com.owncloud.ovarlayiconplugin" FILE "klientdolphinoverlayplugin.json")
    Q_OBJECT

    StatusCache _cache;

public:

    KlientDolphinOverlayPlugin(QObject *parent = nullptr);

    QStringList getOverlays(const QUrl &url) override;

};

#endif // KLIENTDOLPHINOVERLAYPLUGIN_H
