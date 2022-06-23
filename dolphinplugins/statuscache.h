#ifndef STATUSCACHE_H
#define STATUSCACHE_H

#include <QMap>
#include "filestatus.h"

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
    bool isGitDir(const QString &path);

private:
    bool addPath(const QString &path);
    bool isInDir(const QString &dirPath, const QString &filePath);

    friend class OverlayTest;
};

#endif // STATUSCACHE_H
