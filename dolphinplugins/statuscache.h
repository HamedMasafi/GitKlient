#ifndef STATUSCACHE_H
#define STATUSCACHE_H

#include "../src/git/gitmanager.h"

class QUrl;
class QFileInfo ;
class StatusCache
{
    QString _lastDir;
    QMap<QString, FileStatus::Status> _statuses;

public:
    StatusCache();

    FileStatus::Status fileStatus(const QString &path);
    FileStatus::Status pathStatus(const QString &path);
    FileStatus::Status fileStatus(const QFileInfo &fileInfo);

private:
    bool addPath(const QString &path);
    bool isInDir(const QString &dirPath, const QString &filePath);
};

#endif // STATUSCACHE_H
