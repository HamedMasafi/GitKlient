#include "fileactions.h"

#include "dialogs/fileblamedialog.h"
#include "dialogs/filehistorydialog.h"
#include "dialogs/fileviewerdialog.h"
#include "dialogs/searchdialog.h"
#include "git/gitmanager.h"

#include <KLocalizedString>

#include <QAction>
#include <QFileDialog>
#include <QMenu>

FileActions::FileActions(Git::Manager *git, QWidget *parent)
    : QObject(parent), _git{git}, _parent(parent)
{
    _fileMenu = new QMenu(parent);

    actionSaveAs = new QAction(this);
    actionSaveAs->setText(i18n("Save as..."));
    connect(actionSaveAs, &QAction::triggered, this, &FileActions::saveAsFile);
    _fileMenu->addAction(actionSaveAs);

    auto actionView = new QAction(this);
    actionView->setText(i18n("View..."));
    connect(actionView, &QAction::triggered, this, &FileActions::viewFile);
    _fileMenu->addAction(actionView);

    auto actionHistory = new QAction(this);
    actionHistory->setText(i18n("Log..."));
    connect(actionHistory, &QAction::triggered, this, &FileActions::logFile);
    _fileMenu->addAction(actionHistory);


    auto actionBlame = new QAction(this);
    actionBlame->setText(i18n("Blame..."));
    connect(actionBlame, &QAction::triggered, this, &FileActions::blameFile);
    _fileMenu->addAction(actionBlame);

    auto actionSearch = new QAction(this);
    actionSearch->setText(i18n("Search history..."));
    connect(actionSearch, &QAction::triggered, this, &FileActions::search);
    _fileMenu->addAction(actionSearch);
}

void FileActions::viewFile()
{
    FileViewerDialog d(_place, _filePath, _parent);
    d.exec();
}

void FileActions::saveAsFile()
{
    QFileDialog d;
    auto fileName = d.getSaveFileName(_parent);
    if (fileName != QString()) {
        Git::File file{_place, _filePath};
        file.save(fileName);
    }
}

void FileActions::logFile()
{
    Git::File file{_place, _filePath};
    FileHistoryDialog d(_git, file, _parent);
    d.exec();
}

void FileActions::blameFile()
{
//    auto path = _treeModel->fullPath(treeView->currentIndex()) + "/"
//                + listWidget->currentItem()->text();
    Git::File file(_place, _filePath, _git);
    FileBlameDialog d(file, _parent);
    d.exec();
}

void FileActions::search()
{
//    auto path = _treeModel->fullPath(treeView->currentIndex()) + "/"
//                + listWidget->currentItem()->text();
    SearchDialog d(_filePath, Git::Manager::instance(), _parent);
    d.exec();
}
