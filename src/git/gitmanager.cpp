// Copyright (C) 2020 Hamed Masafi <hamed.masafi@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "gitmanager.h"
#include "filestatus.h"

#include "models/branchesmodel.h"
#include "models/logsmodel.h"
#include "models/remotesmodel.h"
#include "models/stashesmodel.h"
#include "models/submodulesmodel.h"
#include "models/tagsmodel.h"

#include <QDebug>
#include <QProcess>
#include <QRegularExpression>
#include <QFile>
#include <QtConcurrent>

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
        loadAsync();

        setIsMerging(QFile::exists(_path + "/.git/MERGE_HEAD"));
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
        fs.setFullPath(_path + "/" + fs.name());
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

bool Manager::isIgnored(const QString &path)
{
    auto tmp = readAllNonEmptyOutput({"check-ignore", path});
    qDebug() << Q_FUNC_INFO << tmp;
    return !tmp.empty();
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

QStringList Manager::fileLog(const QString &fileName) const
{
    return readAllNonEmptyOutput({"log", "--format=format:%H", "--", fileName});
}

QString Manager::diff(const QString &from, const QString &to) const
{
    return runGit({"diff", from, to});
}

QList<FileStatus> Manager::diffBranch(const QString &from) const
{
    auto buffer = QString(runGit({"diff", from, "--name-status"})).split("\n");
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

QList<FileStatus> Manager::diffBranches(const QString &from, const QString &to) const
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

QString Manager::config(const QString &name, ConfigType type) const
{
    QStringList cmd;
    switch (type) {
    case ConfigLocal:
        cmd = QStringList{"config", name};
        break;
    case ConfigGlobal:
        cmd = QStringList{"config", "--global", name};
        break;
    }
    auto list = readAllNonEmptyOutput(cmd);
    if (!list.empty())
        return list.first();

    return QString();
}

void Manager::setConfig(const QString &name, const QString &value, ConfigType type) const
{
    QStringList cmd;
    switch (type) {
    case ConfigLocal:
        cmd = QStringList{"config", name, value};
        break;
    case ConfigGlobal:
        cmd = QStringList{"config", "--global", name, value};
        break;
    }

    runGit(cmd);
}

void Manager::unsetConfig(const QString &name, ConfigType type) const
{
    QStringList cmd{"config", "--unset"};

    if (type == ConfigGlobal)
        cmd.append("--global");

    cmd.append(name);

    runGit(cmd);
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

QString Manager::escapeFileName(const QString &filePath) const
{
    if (filePath.contains(" "))
        return "'" + filePath + "'";
    return filePath;
}

bool load(AbstractGitItemsModel *cache)
{
    cache->load();
    return true;
}
void Manager::loadAsync()
{
    QList<AbstractGitItemsModel *> models;
    if (_loadFlags & LoadStashes)
        models << _stashesCache;
    if (_loadFlags & LoadRemotes)
        models << _remotesModel;
    if (_loadFlags & LoadSubmodules)
        models << _submodulesModel;
    if (_loadFlags & LoadBranches)
        models << _branchesModel;
    if (_loadFlags & LoadLogs)
        models << _logsCache;
    if (_loadFlags & LoadTags)
        models << _tagsModel;

    if (!models.empty())
        QtConcurrent::mapped(models, load);
}

TagsModel *Manager::tagsModel() const
{
    return _tagsModel;
}


StashesModel *Manager::stashesModel() const
{
    return _stashesCache;
}

LogsModel *Manager::logsModel() const
{
    return _logsCache;
}

BranchesModel *Manager::branchesModel() const
{
    return _branchesModel;
}

SubmodulesModel *Manager::submodulesModel() const
{
    return _submodulesModel;
}

RemotesModel *Manager::remotesModel() const
{
    return _remotesModel;
}

const LoadFlags &Manager::loadFlags() const
{
    return _loadFlags;
}

void Manager::setLoadFlags(const LoadFlags &newLoadFlags)
{
    _loadFlags = newLoadFlags;
}

QString Manager::readNote(const QString &branchName) const
{
    return runGit({"notes", "show", branchName});
}

void Manager::saveNote(const QString &branchName, const QString &note) const
{
    runGit({"notes", "add", branchName, "-f", "--message=" + note});
}

Manager::Manager()
    : QObject()
      , _remotesModel{new RemotesModel(this)}
      , _submodulesModel{new SubmodulesModel(this)}
      , _branchesModel{new BranchesModel(this)}
      , _logsCache{new LogsModel(this)}
      , _stashesCache{new StashesModel(this)}
      , _tagsModel{new TagsModel(this)}
{}

Manager::Manager(const QString path)
    : QObject()
      , _remotesModel{new RemotesModel(this)}
      , _submodulesModel{new SubmodulesModel(this)}
      , _branchesModel{new BranchesModel(this)}
      , _logsCache{new LogsModel(this)}
      , _stashesCache{new StashesModel(this)}
      , _tagsModel{new TagsModel(this)}
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

void Manager::init(const QString &path)
{
    _path = path;
    runGit({"init"});
}

bool Manager::isGitDir() const
{
    auto out = runGit(QStringList() << "status");
    return !out.contains("fatal: not a git repository");
}

QByteArray Manager::runGit(const QStringList &args) const
{
//    qDebug().noquote() << "Running: git " << args.join(" ");

    QProcess p;
    p.setProgram("git");
    p.setArguments(args);
    p.setWorkingDirectory(_path);
    p.start();
    p.waitForFinished();
    auto out = p.readAllStandardOutput();
    auto err = p.readAllStandardError();
    Q_UNUSED(err)
//    qDebug() << err;
    return out; // + err;
}

QStringList Manager::ls(const QString &place) const
{
    auto buffer = readAllNonEmptyOutput({"ls-tree", "--name-only", "-r", place});
    QMutableListIterator<QString> it(buffer);
    while (it.hasNext()) {
        auto s = it.next();
        if (s.startsWith("""") && s.endsWith(""""))
            it.setValue(s.mid(1, s.length() - 2));
    }
    return buffer;
}

QString Manager::fileContent(const QString &place, const QString &fileName) const
{
    return runGit({"show", place + ":" + fileName});
}

void Manager::saveFile(const QString &place, const QString &fileName, const QString &localFile) const
{
    auto buffer = runGit({"show", place + ":" + fileName});
    QFile f{localFile};
    if (!f.open(QIODevice::WriteOnly))
        return;
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

        if (b.startsWith("(HEAD detached at"))
            continue;

        branchesList.append(b.trimmed());
    }
    return branchesList;
}

QStringList Manager::remoteBranches() const
{
    QStringList branchesList;
    auto out = QString(runGit({"branch", "--remote", "--list"})).split("\n");

    for (auto &line : out) {
        auto b = line.trimmed();
        if (b.isEmpty())
            continue;
        if (b.startsWith("* "))
            b = b.mid(2);

        if (!b.contains("->"))
        branchesList.append(b.trimmed());
    }
    return branchesList;
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

QList<Stash> Manager::stashes()
{
    QList<Stash> ret;
    auto list = readAllNonEmptyOutput({"stash", "list", "--format=format:%s%m%an%m%ae%m%aD"});
    int id{0};
    for (const auto &item: qAsConst(list)) {
        auto parts = item.split(">");
        if (parts.size() != 4)
            continue;

        auto subject = parts.first();
        Stash stash(this, QStringLiteral("stash@{%1}").arg(id));

        stash._subject = subject;
        stash._authorName = parts.at(1);
        stash._authorEmail = parts.at(2);
        stash._pushTime = QDateTime::fromString(parts.at(3), Qt::RFC2822Date);
        qDebug() << item<< subject<<stash._pushTime;

        ret.append(stash);
        id++;
    }
    return ret;
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

BlameData Manager::blame(const File &file)
{
//    auto logList = logs();
    BlameData b;
    auto lines = readAllNonEmptyOutput({"--no-pager", "blame", "-l", file.fileName()});
    b.reserve(lines.size());

    for (auto &line: lines) {

        BlameDataRow row;
        row.commitHash = line.mid(0, 40);

        auto metaIndex = line.indexOf(")");
        row.code = line.mid(metaIndex + 1);

        auto hash = row.commitHash;
        if (hash.startsWith("^"))
            hash = hash.remove(0, 1);
        auto log = _logsCache->findLogByHash(hash);
        //        if (!log)
        //            qDebug() << "Log not found" << hash;
        row.log = log;
        auto parts = line.split("\t");
        b.append(row);
    }

    return b;
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

        if (line.count(' ') == 2)
            m.setRefName(line.mid(n));
        modules.append(m);
    }
    return modules;
}

bool Manager::addSubmodule(const Submodule &module)
{
    Q_UNUSED(module)
    //TODO:
    return true;
}

void Manager::revertFile(const QString &filePath) const
{
    runGit({"checkout", "--", filePath});
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

void Manager::commit(const QString &message) const
{
    runGit({"commit", "-m", message});
}

void Manager::push() const
{
    runGit({"push", "origin", "master"});
}

void Manager::addFile(const QString &file) const
{
    runGit({"add", file});
}

void Manager::removeFile(const QString &file, bool cached) const
{
    QStringList args;
    args.append("rm");
    if (cached)
        args.append("--cached");
    args.append(file);
    runGit(args);
}

QString Manager::getTopLevelPath() const
{
    //git rev-parse --show-toplevel
    return QString();
}

bool Manager::isMerging() const
{
    return m_isMerging;
}

void Manager::setIsMerging(bool newIsMerging)
{
    if (m_isMerging == newIsMerging)
        return;
    m_isMerging = newIsMerging;
    emit isMergingChanged();
}

} // namespace Git
