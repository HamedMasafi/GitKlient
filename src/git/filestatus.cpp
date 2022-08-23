#include "filestatus.h"
#include <QDebug>
#include <utility>

FileStatus::FileStatus() = default;

FileStatus::FileStatus(QString name, FileStatus::Status status) : _name(std::move(name)),
      _status(status)
{}

const QString &FileStatus::name() const
{
    return _name;
}

FileStatus::Status FileStatus::status() const
{
    return _status;
}

void FileStatus::parseStatusLine(const QString &line)
{
    auto statusX = line.at(0);
    auto statusY = line.at(1);
    auto fileName = line.mid(3);
    _name = fileName;

    setStatus(statusX, statusY);

//    qDebug() << "***=" << line << _status << statusX << statusY;
}

const QString &FileStatus::fullPath() const
{
    return _fullPath;
}

void FileStatus::setFullPath(const QString &newFullPath)
{
    _fullPath = newFullPath;
}

void FileStatus::setStatus(const QString &x, const QString &y)
{
    if (x == "M" || y == "M")
        _status = Modified;
    else if (x == "A")
        _status = Added;
    else if (x == "D")
        _status = Removed;
    else if (x == "R")
        _status = Renamed;
    else if (x == "C")
        _status = Copied;
    else if (x == "U")
        _status = UpdatedButInmerged;
    else if (x == "?")
        _status = Untracked;
    else if (x == "!")
        _status = Ignored;
    else
        _status = Unknown;
}

void FileStatus::setName(const QString &newName)
{
    _name = newName;
}

bool operator==(const FileStatus &f1, const FileStatus &f2)
{
    return f1.name() == f2.name();
}
