#include "fileviewerdialog.h"
#include "git/gitmanager.h"

#include <QDebug>
#include <QFileInfo>
#include <QStandardPaths>

FileViewerDialog::FileViewerDialog(const QString &place, const QString &fileName, QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);

    QStringList images{"png", "bmp", "ico", "jpg", "jpeg"};

    QFileInfo fi(fileName);
    auto suffix = fi.suffix().toLower();
    auto git = Git::Manager::instance();

    if (images.indexOf(suffix) != -1) {
        stackedWidget->setCurrentIndex(1);
        auto p = QStandardPaths::writableLocation(QStandardPaths::TempLocation) +"/klient_img";
        git->saveFile(place, fileName, p);
        QImage img{p};
        labelImage->setPixmap(QPixmap::fromImage(img));
    } else {
        stackedWidget->setCurrentIndex(0);
        plainTextEdit->setPlainText(git->fileContent(place, fileName));
        plainTextEdit->setHighlighting(fileName);
    }
    lineEditBranchName->setText(place);
    lineEditFileName->setText(fileName);
    plainTextEdit->setReadOnly(true);

    setWindowTitle(QStringLiteral("View file: %1").arg(fileName));
}
