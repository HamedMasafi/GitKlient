#include "statuscache.h"

#include "manager.h"
#include <QFileInfo>
#include <QUrl>
#include <QDebug>
#include <QDir>

StatusCache::StatusCache()
{

}
bool StatusCache::addPath(const QString &path)
{
    Git::MiniManager git(path);
    if (!git.isValid())
        return false;

    auto statuses = git.repoFilesStatus();

    for (const auto &s: qAsConst(statuses)) {
        _statuses.insert(git.path() + "/" + s.name(), s.status());
    }
    return true;
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


    if (!addPath(fileInfo.absolutePath()))
        return FileStatus::NoGit;

    if (_statuses.contains(filePath)) {
        return _statuses.value(filePath);
    }

    return FileStatus::Unmodified;
}

bool StatusCache::isGitDir(const QString &path)
{
    Git::MiniManager git(path);
    return git.isValid();
}

FileStatus::Status StatusCache::fileStatus(const QString &filePath)
{
    return fileStatus(QFileInfo(filePath));
}

FileStatus::Status StatusCache::pathStatus(const QString &path)
{
    Git::MiniManager git(path);
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
