#include "fileactions.h"

#include "dialogs/fileblamedialog.h"
#include "dialogs/filehistorydialog.h"
#include "dialogs/fileviewerdialog.h"
#include "dialogs/searchdialog.h"
#include "git/gitmanager.h"

#include <KIO/AccessManager>
#include <KLocalizedString>
#include <KMimeTypeTrader>
#include <KRun>

#include <QAction>
#include <QFileDialog>
#include <QMenu>
#include <QMimeDatabase>

KService::Ptr FileActions::getExternalViewer(const QString &mimeType)
{
    KService::List offers = KMimeTypeTrader::self()->query(mimeType, QStringLiteral("Application"));

    if (!offers.isEmpty()) {
        return offers.first();
    } else {
        return KService::Ptr();
    }
}


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
    _openWithMenu = new QMenu(parent);

    ADD_ACTION(actionView, "View...", &FileActions::viewFile);
    ADD_ACTION(actionOpenWith, "Open with...", &FileActions::openWith);
//    _menu->addMenu(_openWithMenu);
    ADD_ACTION(actionSaveAs, "Save as...", &FileActions::saveAsFile);
    ADD_ACTION(actionHistory, "Log...", &FileActions::logFile);
    ADD_ACTION(actionBlame, "Blame...", &FileActions::blameFile);
    ADD_ACTION(actionSearch, "Search...", &FileActions::search);
}

void FileActions::popup(const QPoint &pos)
{
    _menu->popup(pos);
}

void FileActions::viewFile()
{
    auto d = new FileViewerDialog(_place, _filePath, _parent);
    d->setWindowModality(Qt::ApplicationModal);
    d->setAttribute(Qt::WA_DeleteOnClose, true);
    d->show();
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


KService::Ptr FileActions::getViewer(const QString &mimeType)
{
    // No point in even trying to find anything for application/octet-stream
    if (mimeType == QStringLiteral("application/octet-stream")) {
        return KService::Ptr();
    }

    // Try to get a read-only kpart for the internal viewer
    KService::List offers = KMimeTypeTrader::self()->query(mimeType, QStringLiteral("KParts/ReadOnlyPart"));

    // If we can't find a kpart, try to get an external application
    if (offers.isEmpty()) {
        offers = KMimeTypeTrader::self()->query(mimeType, QStringLiteral("Application"));
    }

    if (!offers.isEmpty()) {
        return offers.first();
    } else {
        return KService::Ptr();
    }
}

void FileActions::openWith()
{
    auto fileName = _git->path() + "/" + _filePath;
    const QList<QUrl> fileUrlList = {QUrl::fromLocalFile(fileName)};
    QMimeDatabase db;
    QMimeType mimeType = db.mimeTypeForFile(fileName);
    auto viewer = getViewer(mimeType.name());
    KRun::runService(*viewer, fileUrlList, nullptr, true);

//    KIO::ApplicationLauncherJob *job = new KIO::ApplicationLauncherJob(viewer);
//    job->setUrls(fileUrlList);
//    job->setUiDelegate(new KIO::JobUiDelegate(KJobUiDelegate::AutoHandlingEnabled, nullptr));
//    // The temporary file will be removed when the viewer application exits.
//    job->setRunFlags(KIO::ApplicationLauncherJob::DeleteTemporaryFiles);
//    job->start();
}
