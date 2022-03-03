#ifndef FILESTATUS_H
#define FILESTATUS_H

#include <QString>

class FileStatus
{
public:
    enum Status
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
        Untracked,
        NoGit
    };

    FileStatus();
    FileStatus(const QString &name, Status status);

    const QString &name() const;
    Status status() const;

    void parseStatusLine(const QString &line);
    const QString &fullPath() const;

    void setFullPath(const QString &newFullPath);

    void setStatus(const QString &statusLetter);
    void setName(const QString &newName);

private:
    QString _fullPath;
    QString _name;
    QString _newName;
    Status _status;

    friend class Manager;
};

#endif // FILESTATUS_H
