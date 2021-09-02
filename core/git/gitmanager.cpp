#include "gitmanager.h"
#include "filestatus.h"

#include <QDebug>
#include <QProcess>
#include <QRegularExpression>
#include <qfile.h>

namespace Git {

const QString &Manager::path() const
{
    return _path;
}

void Manager::setPath(const QString &newPath)
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
        _logs.load();
    }

    Q_EMIT pathChanged();
}

QMap<QString, Manager::ChangeStatus> Manager::changedFiles(const QString &hash) const
{
    QMap<QString, Manager::ChangeStatus> statuses;
    auto buffer = QString(runGit({"show", "--name-status", hash})).split("\n");

    for (auto &line : buffer) {
        if (!line.trimmed().size())
            continue;

        auto parts = line.split("\t");
        if (parts.size() != 2)
            continue;

        if (parts.at(0) == "A")
            statuses.insert(parts.at(1), Added);
        else if (parts.at(0) == "M")
            statuses.insert(parts.at(1), Modified);
        else if (parts.at(0) == "D")
            statuses.insert(parts.at(1), Removed);
        else
            qDebug() << "Unknown file status" << parts.at(0);
    }
    return statuses;
}

QStringList Manager::ignoredFiles() const
{
    return readAllNonEmptyOutput({"check-ignore", "*"});
}

QList<FileStatus> Manager::repoFilesStatus() const
{
    auto buffer = QString(runGit({"status",
                                  "--untracked-files=all",
                                  "--ignored",
                                  "--short",
                                  "--ignore-submodules",
                                  "--porcelain"}))
                      .split("\n");
    QList<FileStatus> files;
    for (auto &item : buffer) {
        if (!item.trimmed().size())
            continue;
        FileStatus fs;
        fs.parseStatusLine(item);
        //        qDebug() << "[STATUS]" << fs.name() << fs.status();
        fs.setFullPath(_path + fs.name());
        files.append(fs);
    }
    return files;
}

QList<Manager::Log *> Manager::log(const QString &branch) const
{
    auto lines = QString(runGit({"--no-pager", "log", branch})).split("\n");

    QList<Log *> logs;
    Log *log{nullptr};
    for (auto &line : lines) {
        if (line.startsWith("commit ")) {
            if (log)
                logs.append(log);
            log = new Log;

            log->hash = line.mid(7).trimmed();
        } else {
            if (!log)
                return QList<Log *>(); // There is an error
            if (line.startsWith("Author:")) {
                log->author = line.mid(8).trimmed();
            } else if (line.startsWith("Date: ")) {
                log->date = line.mid(5).trimmed();
            } else {
                auto l = line.trimmed();
                if (l.size())
                    log->message.append(l + "\n");
            }
        }
    }

    if (log)
        logs.append(log);

    return logs;
}

bool Manager::isValid() const
{
    return _isValid;
}

const LogList &Manager::logs() const
{
    return _logs;
}

bool Manager::addRemote(const QString &name, const QString &url) const
{
    runGit({"remote", "add", name, url});
    return true;
}

bool Manager::removeRemote(const QString &name) const
{
    runGit({"remote", "remove", name});
    return true;
}

bool Manager::renameRemote(const QString &name, const QString &newName) const
{
    runGit({"remote", "rename", name, newName});
    return true;
}

QPair<int, int> Manager::uniqueCommiteOnBranches(const QString &branch1,
                                                 const QString &branch2) const
{
    auto ret = readAllNonEmptyOutput(
        {"rev-list", "--left-right", "--count", branch1 + "..." + branch2});

    if (ret.size() != 1)
        return qMakePair(-1, -1);

    auto parts = ret.first().split('\t');
    if (parts.size() != 2)
        return qMakePair(-1, -1);

    return qMakePair(parts.at(0).toInt(), parts.at(1).toInt());
}

QString Manager::diff(const QString &from, const QString &to)
{
    return runGit({"diff", from, to});
}

QList<FileStatus> Manager::diffBranches(const QString &from, const QString &to)
{
    auto buffer = QString(runGit({"diff", from + ".." + to, "--name-status"})).split("\n");
    QList<FileStatus> files;
    for (auto &item : buffer) {
        if (!item.trimmed().size())
            continue;
        auto parts = item.split("\t");
        if (parts.size() != 2)
            continue;

        FileStatus fs;
        fs.setStatus(parts.at(0));
        fs.setName(parts.at(1));
        files.append(fs);
    }
    return files;
}

QStringList Manager::readAllNonEmptyOutput(const QStringList &cmd) const
{
    QStringList list;
    auto out = QString(runGit(cmd)).split("\n");

    for (auto &line : out) {
        auto b = line.trimmed();
        if (b.isEmpty())
            continue;

        list.append(b.trimmed());
    }
    return list;
}

Manager::Manager() : QObject() {}

Manager::Manager(const QString path) : QObject()
{
    setPath(path);
}

Manager *Manager::instance()
{
    static auto i = new Manager;
    return i;
}

QString Manager::currentBranch() const
{
    auto ret
        = QString(runGit({"rev-parse", "--abbrev-ref", "HEAD"})).replace("\n", "").replace("\r", "");
    return ret;
}

QString Manager::run(const AbstractCommand &cmd) const
{
    return QString(runGit(cmd.generateArgs()));
}

bool Manager::isGitDir() const
{
    auto out = runGit(QStringList() << "status");
    return !out.contains("fatal: not a git repository");
}

QByteArray Manager::runGit(const QStringList &args) const
{
    QProcess p;
    p.setProgram("git");
    p.setArguments(args);
    p.setWorkingDirectory(_path);
    p.start();
    p.waitForFinished();
    auto out = p.readAllStandardOutput();
    auto err = p.readAllStandardError();

    return out; // + err;
}

QString Manager::ls(const QString &place) const
{
    return runGit({"ls-tree", "--name-only", "-r", place});
}

QString Manager::fileContent(const QString &place, const QString &fileName) const
{
    return runGit({"show", place + ":" + fileName});
}

void Manager::saveFile(const QString &place, const QString &fileName, const QString &localFile) const
{
    auto buffer = runGit({"show", place + ":" + fileName});
    QFile f{localFile};
    f.open(QIODevice::WriteOnly);
    f.write(buffer);
    f.close();
}

QStringList Manager::branches() const
{
    QStringList branchesList;
    auto out = QString(runGit({"branch", "--list"})).split("\n");

    for (auto &line : out) {
        auto b = line.trimmed();
        if (b.isEmpty())
            continue;
        if (b.startsWith("* "))
            b = b.mid(2);

        branchesList.append(b.trimmed());
    }
    return branchesList;
}

QStringList Manager::remoteBranches() const
{
    return QString(runGit({"branch", "--remote", "--list"})).split("\n");
}

QStringList Manager::remotes() const
{
    return readAllNonEmptyOutput({"remote"});
}

QStringList Manager::tags() const
{
    return readAllNonEmptyOutput({"tag", "--list"});
}

void Manager::createTag(const QString &name, const QString &message) const
{
    runGit({"tag", "-a", name, "--message", message});
}

QStringList Manager::stashes() const
{
    return readAllNonEmptyOutput({"stash", "list"});
}

void Manager::createStash(const QString &name) const
{
    QStringList args{"stash", "push"};

    if (name != QString())
        args.append({"--message", name});

    auto ret = runGit(args);
    qDebug() << ret;
}

bool Manager::removeStash(const QString &name) const
{
    runGit({"stash", "drop", name});
    return true;
}

bool Manager::applyStash(const QString &name) const
{
    runGit({"stash", "apply", name});
    return true;
}

Remote Manager::remoteDetails(const QString &remoteName)
{
    if (_remotes.contains(remoteName))
        return _remotes.value(remoteName);
    Remote r;
    auto ret = QString(runGit({"remote", "show", remoteName}));
    r.parse(ret);
    _remotes.insert(remoteName, r);
    return r;
}

bool Manager::removeBranch(const QString &branchName) const
{
    auto ret = readAllNonEmptyOutput({"branch", "-D", branchName});
    return true;
}

QList<Submodule> Manager::submodules() const
{
    QList<Submodule> modules;
    auto modulesList = readAllNonEmptyOutput({"submodule", "status"});
    for (auto &line : modulesList) {
        Submodule m;
        m.setCommitHash(line.mid(0, 40));
        auto n = line.lastIndexOf(" ");
        m.setPath(line.mid(41, n - 41));
        m.setRefName(line.mid(n));
        modules.append(m);
    }
    return modules;
}

bool Manager::addSubmodule(const Submodule &module)
{
    return true;
}

QMap<QString, Manager::ChangeStatus> Manager::changedFiles() const
{
    //status --untracked-files=all --ignored --short --ignore-submodules --porcelain
    QMap<QString, Manager::ChangeStatus> statuses;
    auto buffer = QString(runGit({"status", "--short"})).split("\n");

    for (auto &line : buffer) {
        if (!line.trimmed().size())
            continue;

        auto status = line.mid(1, 2).trimmed();
        auto fileName = line.mid(3);

        if (status == "M")
            statuses.insert(fileName, Modified);
        else if (status == "A")
            statuses.insert(fileName, Added);
        else if (status == "D")
            statuses.insert(fileName, Removed);
        else if (status == "R")
            statuses.insert(fileName, Renamed);
        else if (status == "C")
            statuses.insert(fileName, Copied);
        else if (status == "U")
            statuses.insert(fileName, UpdatedButInmerged);
        else if (status == "?")
            statuses.insert(fileName, Untracked);
        else if (status == "!")
            statuses.insert(fileName, Ignored);
        else
            statuses.insert(fileName, Unknown);
    }
    return statuses;
}

void Manager::commit(const QString &message)
{
    runGit({"commit", "-m", message});
}

void Manager::push()
{
    runGit({"push", "origin", "master"});
}

void Manager::addFile(const QString &file)
{
    runGit({"add", file});
}

QString Manager::getTopLevelPath() const
{
    //git rev-parse --show-toplevel
    return QString();
}

} // namespace Git
