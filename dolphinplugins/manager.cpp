#include "manager.h"

#include <QDebug>
#include <gitglobal.h>
#include <QProcess>

namespace Git {

MiniManager::MiniManager(const QString &path)
{
    setPath(path);
}

const QString &MiniManager::path() const
{
    return _path;
}

void MiniManager::setPath(const QString &newPath)
{
    if (_path == newPath)
        return;

    QProcess p;
    p.setProgram("git");
    p.setArguments({"rev-parse", "--show-toplevel"});
    p.setWorkingDirectory(newPath);
    p.start();
    p.waitForFinished();
    auto ret = p.readAllStandardOutput() + p.readAllStandardError();

    if (ret.contains("fatal")) {
        _path = QString();
        _isValid = false;
    } else {
        _path = ret.replace("\n", "");
        _isValid = true;
    }
}

bool MiniManager::isValid() const
{
    return _isValid;
}

QList<FileStatus> MiniManager::repoFilesStatus() const
{
    auto buffer = Git::readAllNonEmptyOutput(_path,
                                             {"status",
                                              "--untracked-files=all",
                                              "--ignored",
                                              "--short",
                                              "--ignore-submodules",
                                              "--porcelain"}, false);

    QList<FileStatus> files;
    QSet<QString> set;
    //TODO: read untrackeds
    for (auto &item : buffer) {
        if (!item.trimmed().size())
            continue;

        if (item.startsWith("??")) {

        }

        FileStatus fs;
        fs.parseStatusLine(item);
        fs.setFullPath(_path + "/" + fs.name());
        if (fs.status() != FileStatus::Untracked)// && !files.contains(fs))
            files.append(fs);

    }
    return files;
}

}
