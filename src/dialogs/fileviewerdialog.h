#ifndef FILEVIEWERDIALOG_H
#define FILEVIEWERDIALOG_H

#include "ui_fileviewerdialog.h"

namespace Git {
class Manager;
class File;
}
class FileViewerDialog : public QDialog, private Ui::FileViewerDialog
{
    Q_OBJECT

public:
    explicit FileViewerDialog(const QString &place, const QString &fileName, QWidget *parent = nullptr);
    explicit FileViewerDialog(Git::Manager *git, const Git::File &file, QWidget *parent = nullptr);
};

#endif // FILEVIEWERDIALOG_H
