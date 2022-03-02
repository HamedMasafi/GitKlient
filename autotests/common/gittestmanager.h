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
};

#endif // GITTESTMANAGER_H
