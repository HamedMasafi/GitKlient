#ifndef GITTESTMANAGER_H
#define GITTESTMANAGER_H

#include <QString>

namespace GitKlientTest {
bool touch(const QString &fileName);
QString getTempPath();
}

class GitTestManager
{
    QString _path;

public:
    GitTestManager();
    bool touch(const QString &fileName);
    bool run(const QString &cmd);

    void init();
    void add(const QString &file = ".");
    void remove(const QString &file, bool cached = true);
    void commit(const QString &message);
    void addToIgnore(const QString &pattern);
    QString absoluteFilePath(const QString &file);

private:
    QString runGit(const QStringList &args);
};

#endif // GITTESTMANAGER_H
