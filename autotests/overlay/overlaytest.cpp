#include "../../dolphinplugins/klientdolphinoverlayplugin.h"
#include "../../src/git/gitmanager.h"
#include "../common/gittestmanager.h"
#include "../../dolphinplugins/statuscache.h"

#include <QtTest/QTest>
#include <QDebug>
#include <QDir>


class OverlayTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void test1();
};


void OverlayTest::test1()
{
    auto p = GitKlientTest::getTempPath();
    Git::Manager m(p);
    m.init(p);
    GitKlientTest::touch(p + "/README.md");
    GitKlientTest::touch(p + "/2");
    m.addFile(p + "/README.md");
    m.addFile(p + "/2");
    m.commit("update readme");
    GitKlientTest::touch(p + "/sample");
    GitKlientTest::touch(p + "/2");
    qDebug() << "p="<<p;
    QDir d(p);
    d.cd(p);
    auto list = d.entryInfoList(QDir::NoDotAndDotDot | QDir::Dirs | QDir::Files);
    StatusCache cache;

    QCOMPARE(cache.fileStatus(p + "/README.md"), FileStatus::Unmodified);
    QCOMPARE(cache.fileStatus(p + "/sample"), FileStatus::Untracked);
    QCOMPARE(cache.fileStatus(p + "/2"), FileStatus::Modified);
}

QTEST_MAIN(OverlayTest)

#include "overlaytest.moc"
