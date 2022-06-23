#include "gittestmanager.h"

#include <QDir>
#include <QProcess>
#include <QStandardPaths>
#include <QUuid>

GitTestManager::GitTestManager()
{
    _path = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/"
            + QUuid::createUuid().toString(QUuid::Id128) + "/";
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

void GitTestManager::init()
{
    runGit({"init"});
}

void GitTestManager::add(const QString &file)
{
    runGit({"add", file});
}

void GitTestManager::remove(const QString &file, bool cached)
{
    QStringList args;
    args << "rm";
    if (cached)
        args << "--cached";
    args << file;
    runGit(args);
}

void GitTestManager::commit(const QString &message)
{
    runGit({"commit", "-m", message});
}

void GitTestManager::addToIgnore(const QString &pattern)
{
    QFile f(_path +"/.gitignore");
    if (!f.open(QIODevice::Text | QIODevice::Append))
        return;

    f.write(pattern.toUtf8() + "\n");
    f.close();
}

QString GitTestManager::absoluteFilePath(const QString &file)
{
    return _path + file;
}

QString GitTestManager::runGit(const QStringList &args)
{
    QProcess p;
    p.setProgram("git");
    p.setArguments(args);
    p.setWorkingDirectory(_path);
    p.start();
    p.waitForFinished();
    auto out = p.readAllStandardOutput();
    //auto err = p.readAllStandardError();
    return out; // + err;
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
