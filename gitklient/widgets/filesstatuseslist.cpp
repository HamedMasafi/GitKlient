#include "filesstatuseslist.h"

FilesStatusesList::FilesStatusesList(QWidget *parent) : QTreeWidget(parent)
{
    setColumnCount(2);
}

const QList<FileStatus> &FilesStatusesList::files() const
{
    return _files;
}

void FilesStatusesList::setFiles(const QList<FileStatus> &newFiles)
{
    _files = newFiles;
}

bool FilesStatusesList::showCheckboxes() const
{
    return _showCheckboxes;
}

void FilesStatusesList::setShowCheckboxes(bool newShowCheckboxes)
{
    _showCheckboxes = newShowCheckboxes;
}
