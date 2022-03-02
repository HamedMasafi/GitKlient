#include "../../dolphinplugins/klientdolphinoverlayplugin.h"
#include "../../src/git/gitmanager.h"
#include "../common/gittestmanager.h"

#include <QtTest/QTest>
#include <QDebug>
#include <QDir>


class OverlayTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void test1();
    void checkIcons();

};


void OverlayTest::test1()
{
    auto p = GitKlientTest::getTempPath();
    Git::Manager m(p);
    m.init(p);
    GitKlientTest::touch(p + "/README.md");
    m.addFile(p + "/README.md");
    m.commit("update readme");
    GitKlientTest::touch(p + "/sample");
    qDebug() << "p="<<p;
    QDir d(p);
    d.cd(p);
    auto list = d.entryInfoList(QDir::NoDotAndDotDot | QDir::Dirs | QDir::Files);
    KlientDolphinOverlayPlugin plugin;
    QMap<QString, QStringList> icons;
    for (auto &fi: list) {
        auto icon = plugin.getOverlays(QUrl::fromLocalFile(fi.absoluteFilePath()));
        qDebug() << fi.absoluteFilePath() << icon;
        icons.insert(fi.absoluteFilePath(), icon);
    }

    QCOMPARE(icons.value(p + "/README.md"), {"git-status-update"});
}

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
