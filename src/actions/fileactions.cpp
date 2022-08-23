#include "fileactions.h"

#include "dialogs/fileblamedialog.h"
#include "dialogs/filehistorydialog.h"
#include "dialogs/fileviewerdialog.h"
#include "dialogs/searchdialog.h"
#include "diffwindow.h"
#include "git/gitfile.h"
#include "git/gitmanager.h"
#include "mergewindow.h"

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

    if (!offers.isEmpty())
        return offers.first();
    return KService::Ptr();
}

const QString &FileActions::place() const
{
    return _place;
}

void FileActions::setPlace(const QString &newPlace)
{
    _place = newPlace;

    setActionEnabled(_actionView, !_filePath.isEmpty());
    setActionEnabled(_actionOpenWith, !_filePath.isEmpty());
    setActionEnabled(_actionDiffWithHead, !_filePath.isEmpty());
    setActionEnabled(_actionMergeWithHead, !_filePath.isEmpty());
    setActionEnabled(_actionSaveAs, !_filePath.isEmpty());
    setActionEnabled(_actionHistory, !_filePath.isEmpty());
    setActionEnabled(_actionBlame, !_filePath.isEmpty());
    setActionEnabled(_actionSearch, !_filePath.isEmpty());
}

const QString &FileActions::filePath() const
{
    return _filePath;
}

void FileActions::setFilePath(const QString &newFilePath)
{
    _filePath = newFilePath;

    setActionEnabled(_actionView, !_place.isEmpty());
    setActionEnabled(_actionOpenWith, !_place.isEmpty());
    setActionEnabled(_actionDiffWithHead, !_place.isEmpty());
    setActionEnabled(_actionMergeWithHead, !_place.isEmpty());
    setActionEnabled(_actionSaveAs, !_place.isEmpty());
    setActionEnabled(_actionHistory, !_place.isEmpty());
    setActionEnabled(_actionBlame, !_place.isEmpty());
    setActionEnabled(_actionSearch, !_place.isEmpty());
}

FileActions::FileActions(Git::Manager *git, QWidget *parent) : AbstractActions(git, parent)
{
    _openWithMenu = new QMenu(parent);

    _actionView = addAction(i18n("View..."),this, &FileActions::viewFile,  false, true);
    _actionOpenWith = addAction(i18n("Open with..."),this, &FileActions::openWith,  false, true);

    _actionDiffWithHead = addAction(i18n("Diff with HEAD..."),this, &FileActions::diffWithHead,  false, true);
    _actionMergeWithHead = addAction(i18n("Merge with HEAD..."),this, &FileActions::mergeWithHead,  false, true);

    _actionSaveAs = addAction(i18n("Save as..."),this, &FileActions::saveAsFile,  false, true);
    _actionHistory = addAction(i18n("Log..."),this, &FileActions::logFile,  false, true);
    _actionBlame = addAction(i18n("Blame..."),this, &FileActions::blameFile,  false, true);
    _actionSearch = addAction(i18n("Search..."),this, &FileActions::search,  false, true);
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
    auto fileName = QFileDialog::getSaveFileName(_parent);
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
    KService::List offers = KMimeTypeTrader::self()->query(mimeType,
                                                           QStringLiteral("KParts/ReadOnlyPart"));

    // If we can't find a kpart, try to get an external application
    if (offers.isEmpty()) {
        offers = KMimeTypeTrader::self()->query(mimeType, QStringLiteral("Application"));
    }

    if (!offers.isEmpty())
        return offers.first();
    return KService::Ptr();
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

void FileActions::diffWithHead()
{
    Git::File oldFile{_place, _filePath};
    Git::File newFile{_git->path() + "/" + _filePath};

    auto d = new DiffWindow(oldFile, newFile);
    d->showModal();
}

void FileActions::mergeWithHead()
{
    auto d = new MergeWindow(MergeWindow::NoParams);

    auto p = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/ggggg";
    Git::File f{_place, _filePath};
    f.save(p);

    d->setFilePathBase(p);
    d->setFilePathLocal(_git->path() + "/" + _filePath);
    d->setFilePathRemote(p);
    d->setFilePathResult(_git->path() + "/" + _filePath);
    d->load();

    d->showModal();
}
