#ifndef DIFFWINDOW_H
#define DIFFWINDOW_H

#include "core/appmainwindow.h"
#include <git/gitfile.h>
#include <QObject>

class DiffTreeModel;
class DiffWidget;
class DiffTreeView;
class FilesModel;
class DiffWindow : public AppMainWindow
{
    Q_OBJECT

    Git::File _oldFile;
    Git::File _newFile;

    QString _oldBranch;
    QString _newBranch;

    QString _leftDir, _rightDir;

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
    Storage _leftStorage{NoStorage};
    Storage _rightStorage{NoStorage};

public:
    explicit DiffWindow();
    DiffWindow(Git::Manager *git);
    DiffWindow(const Git::File &oldFile, const Git::File &newFile);
    DiffWindow(Git::Manager *git, const QString &oldBranch, const QString &newBranch);
    DiffWindow(const QString &oldDir, const QString &newDir);

private slots:
    void fileOpen();
    void settings();
    void on_treeView_fileSelected(const QString &file);

private:
    void compareDirs();
};

#endif // DIFFWINDOW_H
