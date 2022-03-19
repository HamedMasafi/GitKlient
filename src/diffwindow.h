#ifndef DIFFWINDOW_H
#define DIFFWINDOW_H

#include "core/mainwindow.h"
#include <git/gitfile.h>
#include <QObject>

class DiffTreeModel;
class DiffWidget;
class DiffTreeView;
class FilesModel;
class DiffWindow : public MainWindow
{
    Q_OBJECT

    Git::File _oldFile;
    Git::File _newFile;

    QString _oldBranch;
    QString _newBranch;

    QString _oldDir, _newDir;

    FilesModel *_filesModel;
    DiffTreeModel *_diffModel;
    DiffWidget *_diffWidget;
    DiffTreeView *_treeView;

    void initActions();
    void init(bool showSideBar);

    enum Mode {
        None,
        Dirs,
        Files
    };
    enum Storage {
        NoStorage,
        FileSystem,
        Git
    };

    Mode _mode{None};
    Storage _storage{NoStorage};

public:
    explicit DiffWindow();
    DiffWindow(const Git::File &oldFile, const Git::File &newFile);
    DiffWindow(const QString &oldBranch, const QString &newBranch);

private slots:
    void fileOpen();
    void on_treeView_fileSelected(const QString &file);

private:
    void compareDirs();
};

#endif // DIFFWINDOW_H
