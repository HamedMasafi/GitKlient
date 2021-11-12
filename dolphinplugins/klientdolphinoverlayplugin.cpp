#include "klientdolphinoverlayplugin.h"

#include "../src/git/gitmanager.h"
#include <QUrl>
#include <QDebug>
#include <QDir>

KlientDolphinOverlayPlugin::KlientDolphinOverlayPlugin(QObject *parent)
    :
#ifdef GK_TEST
      QObject(parent)
#else
      KOverlayIconPlugin(parent)
#endif
{}

QString icon(const FileStatus::Status &status)
{
    switch (status) {
    case FileStatus::Added:
        return "git-status-add";
    case FileStatus::Ignored:
        return "git-status-ignored";
    case FileStatus::Modified:
        return "git-status-modified";
    case FileStatus::Removed:
        return "git-status-remove";
    case FileStatus::Renamed:
        return "git-status-renamed";
    case FileStatus::Unknown:
    case FileStatus::Untracked:
        return "git-status-unknown";
    case FileStatus::Copied:
    case FileStatus::UpdatedButInmerged:
    case FileStatus::Unmodified:
        return "git-status-update";
    }
    return "git-status-update";
}

QString KlientDolphinOverlayPlugin::pathIcon(const QString &path)
{
    QSet<FileStatus::Status> statues;
    for (auto i = _statusCache.begin(); i != _statusCache.end(); ++i) {
        if (i.key().startsWith(path))
            statues.insert(i.value());
    }
    if (!statues.size() )
        return icon(FileStatus::Unknown);
    else if (statues.size() == 1)
        return icon (statues.values().first());
    else {
        return icon(FileStatus::Modified);
    }
}

QStringList KlientDolphinOverlayPlugin::getOverlays(const QUrl &url)
{
    auto p = url.toLocalFile();

//    qDebug() << "init" << _lastDir << p ;
    if (!_lastDir.isEmpty() && p.startsWith(_lastDir)) {
        if (_statusCache.contains(p))
            return {icon(_statusCache.value(p))};

        QDir d;
        if (d.exists(p))
            return {pathIcon(p)};
        //return {icon(_statusCache.value(p, FileStatus::Unmodified))};
    }

//    if (_statusCache.contains(p)) {
//        qDebug() << "p found" << p;
//        return {icon(_statusCache.value(p))};
//    }

    Git::Manager git;
    git.setPath(p);
//    qDebug() << "setpath=" << p << git.isValid() << git.path();

    if (!git.isValid()) {
        _lastDir = QString();
        return {};
    }

    _lastDir = git.path();
    auto statuses = git.repoFilesStatus();

    for (auto &s: statuses) {
        qDebug() << "insert" << s.name() << git.path() << s.status();
        _statusCache.insert(git.path() + "/" + s.name(), s.status());
        if (git.path() + "/" + s.name() == p) {
            qDebug() <<"Found"<<s.status();
            return {icon(s.status())};
        }
    }
    return {};
}

#include "klientdolphinoverlayplugin.moc"
