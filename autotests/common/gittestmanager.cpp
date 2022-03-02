#include "gittestmanager.h"

#include <QDir>
#include <QStandardPaths>
#include <QUuid>

GitTestManager::GitTestManager()
{
    _path = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/"
            + QUuid::createUuid().toString(QUuid::Id128);
    QDir d;
    d.mkpath(_path);
}

bool GitTestManager::touch(const QString &fileName)
{
    QFile f(_path + "/" + fileName);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;
    f.write(QUuid::createUuid().toString(QUuid::Id128).toLatin1());
    f.close();
    return true;
}

bool GitTestManager::run(const QString &cmd)
{
    auto parts = cmd.split(' ');
    return true;
}

namespace GitKlientTest {
bool touch(const QString &fileName)
{
    QFile f(fileName);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;
    f.write(QUuid::createUuid().toString(QUuid::Id128).toLatin1());
    f.close();
    return true;
}

QString getTempPath()
{
    auto path = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/"
            + QUuid::createUuid().toString(QUuid::Id128);
    QDir d;
    d.mkpath(path);
    return path;
}
} // namespace GitKlientTest
