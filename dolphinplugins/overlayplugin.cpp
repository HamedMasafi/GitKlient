#include "overlayplugin.h"

#include "../src/git/gitmanager.h"
#include <QUrl>
#include <QDebug>
#include <QDir>

OverlayPlugin::OverlayPlugin(QObject *parent) : KOverlayIconPlugin(parent)
{}

QString icon(const FileStatus::Status &status)
{
    switch (status) {
    case FileStatus::Added:
        return "git-status-added";
    case FileStatus::Ignored:
        return "git-status-ignored";
    case FileStatus::Modified:
        return "git-status-modified";
    case FileStatus::Removed:
        return "git-status-removed";
    case FileStatus::Renamed:
        return "git-status-renamed";
    case FileStatus::Unknown:
    case FileStatus::Untracked:
        return "git-status-unknown";
    case FileStatus::Copied:
    case FileStatus::UpdatedButInmerged:
    case FileStatus::Unmodified:
        return "git-status-update";
    case FileStatus::NoGit:
        return "";
    default:
        qWarning() << "Unknown icon" << status;
    }
    return "git-status-update";
}

QStringList OverlayPlugin::getOverlays(const QUrl &url)
{
    if (!url.isLocalFile())
        return {icon(FileStatus::NoGit)};

    QFileInfo fi(url.toLocalFile());
    if (fi.isDir()) {
        return {icon(_cache.pathStatus(fi.absoluteFilePath()))};
    }

    return {icon(_cache.fileStatus(fi))};
}

#include "overlayplugin.moc"
