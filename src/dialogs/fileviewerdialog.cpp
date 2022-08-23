#include "fileviewerdialog.h"
#include "git/gitmanager.h"

#include <KStandardAction>
#include <KParts/OpenUrlArguments>
#include <KLocalizedString>
#include <KMimeTypeTrader>
#include <KXMLGUIFactory>
#include <KActionCollection>

#include <QDebug>
#include <QFileInfo>
#include <QMimeData>
#include <QMimeDatabase>
#include <QProgressDialog>
#include <QSettings>
#include <QStandardPaths>
#include <QStyle>

FileViewerDialog::FileViewerDialog(const QString &place, const QString &fileName, QWidget *parent)
    :  KParts::MainWindow(parent)
{
    setupUi(this);
    showFile(Git::File(place, fileName));
    _git = Git::Manager::instance();
    QSettings s;
    restoreGeometry(s.value("FileViewerDialog_Geometry").toByteArray());
    KStandardAction::close(this, &QMainWindow::close, actionCollection());

    setupGUI(ToolBar, "gitklientfileviewerui.rc");
}

FileViewerDialog::FileViewerDialog(Git::Manager *git, const Git::File &file, QWidget *parent)
    : KParts::MainWindow(parent), _git(git)
{
    setupUi(this);
    showFile(Git::File(file));
    QSettings s;
    restoreGeometry(s.value("FileViewerDialog_Geometry").toByteArray());
    KStandardAction::close(this, &QMainWindow::close, actionCollection());

    setupGUI(ToolBar, "gitklientfileviewerui.rc");
}

FileViewerDialog::~FileViewerDialog()
{
    QSettings s;
    s.setValue("FileViewerDialog_Geometry", saveGeometry());

    if (_filePath != QString() && QFile::exists(_filePath))
        QFile::remove(_filePath);

    if (m_part) {
        QProgressDialog progressDialog(this);
        progressDialog.setWindowTitle(i18n("Closing preview"));
        progressDialog.setLabelText(i18n("Please wait while the preview is being closed..."));

        progressDialog.setMinimumDuration(500);
        progressDialog.setModal(true);
        progressDialog.setCancelButton(nullptr);
        progressDialog.setRange(0, 0);

        // #261785: this preview dialog is not modal, so we need to delete
        //          the previewed file ourselves when the dialog is closed;

        m_part.data()->closeUrl();

//        if (!m_fileName.isEmpty()) {
//            QFile::remove(m_fileName);
//        }
    }

    guiFactory()->removeClient(m_part);
    delete m_part;
}

void FileViewerDialog::showFile(const Git::File &file)
{
    QMimeDatabase mimeDatabase;
    auto fn = file.fileName().mid(file.fileName().lastIndexOf("/") + 1);
    auto mime = mimeDatabase.mimeTypeForFile(fn, QMimeDatabase::MatchExtension);
    _filePath = file.fileName();
    _filePath = _filePath.mid(_filePath.lastIndexOf("/") + 1);
    _filePath.prepend(QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/");

    lineEditBranchName->setText(file.place());
    lineEditFileName->setText(file.fileName());
    plainTextEdit->setReadOnly(true);
    setWindowTitle(QStringLiteral("View file: %1").arg(file.fileName()));
    setWindowFilePath(file.fileName());
    labelFileIcon->setPixmap(
        QIcon::fromTheme(mime.iconName())
            .pixmap(style()->pixelMetric(QStyle::PixelMetric::PM_SmallIconSize)));

    auto ptr = getInternalViewer(mime.name());
    if (ptr && ptr->isValid()) {
        file.save(_filePath);
        if (viewInInternalViewer(ptr, _filePath, mime))
            return;
    }

    if (mime.name().startsWith("text/"))
        showInEditor(file);
    else if (mime.name().startsWith("image/"))
        showAsImage(file);
    else {
        if (!ptr || !ptr->isValid()) {
            showInEditor(file);
            qDebug() << "fallback to text mode";
        } else {
            file.save(_filePath);
            if (!viewInInternalViewer(ptr, _filePath, mime))
                showInEditor(file);
        }
    }
    qDebug() << "mime is" << mime.name() << fn << mimeDatabase.suffixForFileName(fn)
             << stackedWidget->currentIndex();

}

void FileViewerDialog::showInEditor(const Git::File &file)
{
    stackedWidget->setCurrentIndex(0);
    plainTextEdit->setPlainText(file.content());
    plainTextEdit->setHighlighting(file.fileName());
}

void FileViewerDialog::showAsImage(const Git::File &file)
{
    stackedWidget->setCurrentIndex(1);
    auto p = QStandardPaths::writableLocation(QStandardPaths::TempLocation) +"/klient_img";
    file.save(p);
    QImage img{p};
    labelImage->setPixmap(QPixmap::fromImage(img));
}

KService::Ptr FileViewerDialog::getInternalViewer(const QString& mimeType)
{
    // No point in even trying to find anything for application/octet-stream
    if (mimeType == QLatin1String("application/octet-stream")) {
        return KService::Ptr();
    }

    // Try to get a read-only kpart for the internal viewer
    KService::List offers = KMimeTypeTrader::self()->query(mimeType, QStringLiteral("KParts/ReadOnlyPart"));

    qDebug() << offers.size() << "offer(s) found for" << mimeType;
    for (const auto &offer: qAsConst(offers))
        qDebug() << " *" << offer->name() << offer->genericName();
    /*auto arkPartIt = std::find_if(offers.begin(), offers.end(), [](KService::Ptr service) {
        return service->storageId() == QLatin1String("ark_part.desktop");
    });

    // Use the Ark part only when the mime type matches an archive type directly.
    // Many file types (e.g. Open Document) are technically just archives
    // but browsing their internals is typically not what the user wants.
    if (arkPartIt != offers.end()) {
        // Not using hasMimeType() as we're explicitly not interested in inheritance.
        if (!(*arkPartIt)->mimeTypes().contains(mimeType)) {
            offers.erase(arkPartIt);
        }
    }*/

    // Skip the KHTML part
    auto khtmlPart = std::find_if(offers.begin(), offers.end(), [](KService::Ptr service) {
        return service->desktopEntryName() == QLatin1String("khtml");
    });

    if (khtmlPart != offers.end()) {
        offers.erase(khtmlPart);
    }

    if (!offers.isEmpty()) {
        return offers.first();
    } else {
        return KService::Ptr();
    }
}

KService::Ptr FileViewerDialog::getExternalViewer(const QString &mimeType)
{
    qDebug() << mimeType;
    KService::List offers = KMimeTypeTrader::self()->query(mimeType);

    if (!offers.isEmpty()) {
        return offers.first();
    } else {
        return KService::Ptr();
    }
}

bool FileViewerDialog::viewInInternalViewer(const KService::Ptr& viewer, const QString& fileName, const QMimeType &mimeType)
{
    Q_UNUSED(mimeType)
    // Set icon and comment for the mimetype.
//    m_iconLabel->setPixmap(QIcon::fromTheme(mimeType.iconName()).pixmap(style()->pixelMetric(QStyle::PixelMetric::PM_SmallIconSize)));
//    m_commentLabel->setText(mimeType.comment());

    // Create the ReadOnlyPart instance.
    QString error;
    m_part = viewer->createInstance<KParts::ReadOnlyPart>(widgetContainer, widgetContainer, QVariantList(), &error);

    if (!m_part.data()) {
        qDebug() << "m_part is null" << error;
        return false;
    }

    // Insert the KPart into its placeholder.
    kPartLayout->addWidget(m_part.data()->widget());
    stackedWidget->setCurrentIndex(2);
//    layout()->replaceWidget(plainTextEdit, m_part.data()->widget());
/*
    QAction* action = actionCollection()->addAction(QStringLiteral("help_about_kpart"));
    const KPluginMetaData partMetaData = m_part->metaData();
    const QString iconName = partMetaData.iconName();
    if (!iconName.isEmpty()) {
        action->setIcon(QIcon::fromTheme(iconName));
    }
    action->setText(i18nc("@action", "About Viewer Component"));
    connect(action, &QAction::triggered, this, &ArkViewer::aboutKPart);
*/
    createGUI(m_part.data());
//    setAutoSaveSettings(QStringLiteral("Viewer"), true);

    m_part.data()->openUrl(QUrl::fromLocalFile(fileName));
    m_part.data()->widget()->setFocus();
//    m_fileName = fileName;

    return true;
}
