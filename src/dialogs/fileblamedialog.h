#ifndef FILEBLAMEDIALOG_H
#define FILEBLAMEDIALOG_H

#include "../core/appdialog.h"
#include "ui_fileblamedialog.h"
#include "git/gitfile.h"

namespace Git {
class Manager;
}

class FileBlameDialog : public AppDialog, private Ui::FileBlameDialog
{
    Q_OBJECT
    Git::Manager *_git;
    QString _fileName;
    Git::File _file;

public:
    explicit FileBlameDialog(Git::Manager *git, const QString &fileName, QWidget *parent = nullptr);
    explicit FileBlameDialog(const Git::File &file, QWidget *parent = nullptr);

private slots:
    void on_plainTextEdit_blockSelected();
};

#endif // FILEBLAMEDIALOG_H
