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

const QString &FileActions::place() const
{
    return _place;
}

void FileActions::setPlace(const QString &newPlace)
{
    _place = newPlace;
}

const QString &FileActions::filePath() const
{
    return _filePath;
}

void FileActions::setFilePath(const QString &newFilePath)
{
    _filePath = newFilePath;
}

FileActions::FileActions(Git::Manager *git, QWidget *parent)
    : AbstractActions(git, parent)
{
    actionSaveAs = new QAction(this);
    actionSaveAs->setText(i18n("Save as..."));
    connect(actionSaveAs, &QAction::triggered, this, &FileActions::saveAsFile);
    _menu->addAction(actionSaveAs);

    auto actionView = new QAction(this);
    actionView->setText(i18n("View..."));
    connect(actionView, &QAction::triggered, this, &FileActions::viewFile);
    _menu->addAction(actionView);

    auto actionHistory = new QAction(this);
    actionHistory->setText(i18n("Log..."));
    connect(actionHistory, &QAction::triggered, this, &FileActions::logFile);
    _menu->addAction(actionHistory);


    auto actionBlame = new QAction(this);
    actionBlame->setText(i18n("Blame..."));
    connect(actionBlame, &QAction::triggered, this, &FileActions::blameFile);
    _menu->addAction(actionBlame);

    auto actionSearch = new QAction(this);
    actionSearch->setText(i18n("Search history..."));
    connect(actionSearch, &QAction::triggered, this, &FileActions::search);
    _menu->addAction(actionSearch);
}

void FileActions::popup(const QPoint &pos)
{
    _menu->popup(pos);
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
