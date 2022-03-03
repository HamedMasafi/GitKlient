#include "statuscache.h"

#include <QFileInfo>
#include <QUrl>
#include <QDebug>
#include <QDir>

StatusCache::StatusCache()
{

}
void StatusCache::addPath(const QString &path)
{
    Git::Manager git(path);
    if (!git.isValid())
        return;

    auto statuses = git.repoFilesStatus();

    for (const auto &s: qAsConst(statuses)) {
        _statuses.insert(git.path() + "/" + s.name(), s.status());
//        if (s.status() != FileStatus::Ignored)
//            qDebug() << s.status() << git.path() << s.name();
    }
}

bool StatusCache::isInDir(const QString &dirPath, const QString &filePath)
{
    if (dirPath.endsWith("/"))
        return filePath.lastIndexOf("/") == dirPath.size() - 1;
    return filePath.lastIndexOf("/") == dirPath.size();
}

FileStatus::Status StatusCache::fileStatus(const QFileInfo &fileInfo)
{
    auto filePath = fileInfo.absoluteFilePath();

    if (!_lastDir.isEmpty() && isInDir(_lastDir, filePath)) {
        if (_statuses.contains(filePath)) {
            return _statuses.value(filePath);
        } else
            return FileStatus::Unknown;
    }


    addPath(fileInfo.absolutePath());

    if (_statuses.contains(filePath)) {
        return _statuses.value(filePath);
    }

    return FileStatus::Unmodified;
}

FileStatus::Status StatusCache::fileStatus(const QString &filePath)
{
    return fileStatus(QFileInfo(filePath));
}

FileStatus::Status StatusCache::pathStatus(const QString &path)
{
    Git::Manager git(path);
    if (!git.isValid())
        return FileStatus::NoGit;

    auto statuses = git.repoFilesStatus();
    FileStatus::Status status = FileStatus::Unmodified;

    for (const auto &s: qAsConst(statuses)) {
        auto filePath = git.path() + "/" + s.name();

        if (!filePath.startsWith(path)) {
            continue;
        }

        if (status == FileStatus::Unmodified) {
            status = s.status();
        } else if (status != s.status()) {
            return FileStatus::Modified;
        }
    }
    return status;
}
