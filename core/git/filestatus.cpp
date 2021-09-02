#include "filestatus.h"
#include <QDebug>

FileStatus::FileStatus()
{

}

FileStatus::FileStatus(const QString &name, FileStatus::Status status) : _name(name),
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
    auto status = line.at(1);
    auto fileName = line.mid(3);
    _name = fileName;

    setStatus(status);

//    qDebug() << "line=" << line<< status<<_status <<fileName;
}

const QString &FileStatus::fullPath() const
{
    return _fullPath;
}

void FileStatus::setFullPath(const QString &newFullPath)
{
    _fullPath = newFullPath;
}

void FileStatus::setStatus(const QString &statusLetter)
{
    if (statusLetter == "M")
        _status = Modified;
    else if (statusLetter == "A")
        _status = Added;
    else if (statusLetter == "D")
        _status = Removed;
    else if (statusLetter == "R")
        _status = Renamed;
    else if (statusLetter == "C")
        _status = Copied;
    else if (statusLetter == "U")
        _status = UpdatedButInmerged;
    else if (statusLetter == "?")
        _status = Untracked;
    else if (statusLetter == "!")
        _status = Ignored;
    else
        _status = Unknown;
}

void FileStatus::setName(const QString &newName)
{
    _name = newName;
}
