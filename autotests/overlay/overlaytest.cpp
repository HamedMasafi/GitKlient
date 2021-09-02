#include "../../dolphinplugins/klientdolphinoverlayplugin.h"
#include <QtTest/QTest>
#include <QDebug>
#include <QDir>


class OverlayTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void checkIcons();

};


void OverlayTest::checkIcons()
{
    QDir d("/doc/dev/gitklient/");
    auto list = d.entryInfoList();
    KlientDolphinOverlayPlugin plugin;

    for (auto &fi: list) {
        auto icon = plugin.getOverlays(QUrl::fromLocalFile(fi.absoluteFilePath()));
        qDebug() << fi.absoluteFilePath() << icon;
    }
}

QTEST_MAIN(OverlayTest)


#include "overlaytest.moc"
