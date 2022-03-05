#ifndef KLIENTDOLPHINOVERLAYPLUGIN_H
#define KLIENTDOLPHINOVERLAYPLUGIN_H

#include "../src/git/filestatus.h"
#include "statuscache.h"
#include <KOverlayIconPlugin>
#include <QCache>
#include <QMap>

class OverlayPlugin : public KOverlayIconPlugin
{
    Q_PLUGIN_METADATA(IID "org.kfe.gitklient.ovarlayiconplugin" FILE "gitklientoverlayplugin.json")
    Q_OBJECT

    StatusCache _cache;

public:

    OverlayPlugin(QObject *parent = nullptr);

    QStringList getOverlays(const QUrl &url) override;

};

#endif // KLIENTDOLPHINOVERLAYPLUGIN_H
