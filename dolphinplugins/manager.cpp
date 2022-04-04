#include "manager.h"

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
                                              "--porcelain"});

    QList<FileStatus> files;
    for (auto &item : buffer) {
        if (!item.trimmed().size())
            continue;
        FileStatus fs;
        fs.parseStatusLine(item);
        //        qDebug() << "[STATUS]" << fs.name() << fs.status();
        fs.setFullPath(_path + "/" + fs.name());
        files.append(fs);
    }
    return files;
}

}
