#ifndef GITMANAGER_H
#define GITMANAGER_H

#include "gitloglist.h"
#include "abstractcommand.h"
#include "filestatus.h"
#include "gitremote.h"
#include "gitsubmodule.h"
#include "blamedata.h"
#include "gitfile.h"

#include <QString>
#include <QObject>

namespace Git {

class Manager : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString path READ path WRITE setPath NOTIFY pathChanged)

    QString _path;
    bool _isValid{false};
    LogList _logs;
    QMap<QString, Remote> _remotes;

public:
    enum ChangeStatus
    {
        Unknown,
        Unmodified,
        Modified,
        Added,
        Removed,
        Renamed,
        Copied,
        UpdatedButInmerged,
        Ignored,
        Untracked
    };

    struct Log
    {
        QString hash;
        QString author;
        QString date;
        QString message;
    };

    Manager();
    Manager(const QString path);
    static Manager *instance();

    QString currentBranch() const;

    QString run(const AbstractCommand &cmd) const;

    bool isGitDir() const;
    QByteArray runGit(const QStringList &args) const;
    QStringList ls(const QString &place) const;
    QString fileContent(const QString &place, const QString &fileName) const;
    void saveFile(const QString &place, const QString &fileName, const QString &localFile) const;
    QStringList branches() const;
    QStringList remoteBranches() const;
    QStringList remotes() const;

    QStringList tags() const;
    void createTag(const QString &name, const QString &message) const;

    QStringList stashes() const;
    void createStash(const QString &name = QString()) const;
    bool removeStash(const QString &name) const;
    bool applyStash(const QString &name) const;

    Remote remoteDetails(const QString &remoteName);
    bool removeBranch(const QString &branchName) const;

    BlameData blame(const File &file);

    QList<Git::Submodule> submodules() const;
    bool addSubmodule(const Git::Submodule &module);

    QMap<QString, Manager::ChangeStatus> changedFiles() const;
    void commit(const QString &message);
    void push();
    void addFile(const QString &file);
    QString getTopLevelPath() const;

    const QString &path() const;
    void setPath(const QString &newPath);

    QMap<QString, ChangeStatus> changedFiles(const QString &hash) const;

    QStringList ignoredFiles() const;
    QList<FileStatus> repoFilesStatus() const;

    QList<Log *> log(const QString &branch) const;
    bool isValid() const;

    const LogList &logs();

    bool addRemote(const QString &name, const QString &url) const;
    bool removeRemote(const QString &name) const;
    bool renameRemote(const QString &name, const QString &newName) const;

    QPair<int, int> uniqueCommiteOnBranches(const QString &branch1, const QString &branch2) const;

    QStringList fileLog(const QString &fileName) const;

    QString diff(const QString &from, const QString &to);
    QList<FileStatus> diffBranches(const QString &from, const QString &to);
signals:
    void pathChanged();

private:
    QStringList readAllNonEmptyOutput(const QStringList &cmd) const;
};

} // namespace Git

#endif // GITMANAGER_H
