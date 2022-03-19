#ifndef FILEBLAMEDIALOG_H
#define FILEBLAMEDIALOG_H

#include "../core/dialog.h"
#include "ui_fileblamedialog.h"
#include "git/gitfile.h"

namespace Git {
class Manager;
}

class FileBlameDialog : public Dialog, private Ui::FileBlameDialog
{
    Q_OBJECT
    Git::Manager *_git;
    QString _fileName;
    Git::File _file;

public:
    explicit FileBlameDialog(Git::Manager *git, const QString &fileName, QWidget *parent = nullptr);
    explicit FileBlameDialog(const Git::File &file, QWidget *parent = nullptr);
};

#endif // FILEBLAMEDIALOG_H
