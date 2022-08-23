#ifndef FILESSTATUSESLIST_H
#define FILESSTATUSESLIST_H

#include <QTreeWidget>
#include "git/filestatus.h"

class FilesStatusesList : public QTreeWidget
{
    Q_OBJECT
public:
    explicit FilesStatusesList(QWidget *parent = nullptr);

    const QList<FileStatus> &files() const;
    void setFiles(const QList<FileStatus> &newFiles);
    bool showCheckboxes() const;
    void setShowCheckboxes(bool newShowCheckboxes);

private:
    QList<FileStatus> _files;
    bool _showCheckboxes{false};
};

#endif // FILESSTATUSESLIST_H
