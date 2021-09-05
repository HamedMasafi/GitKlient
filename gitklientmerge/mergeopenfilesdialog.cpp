#include "openfilesdialog.h"

#include <QFileDialog>

OpenFilesDialog::OpenFilesDialog(QWidget *parent) :
      QDialog(parent)
{
    setupUi(this);
}

QString OpenFilesDialog::filePathLocal() const
{
    return lineEditLocalFile->text();
}

void OpenFilesDialog::setFilePathLocal(const QString &newFilePathLocal)
{
    lineEditLocalFile->setText(newFilePathLocal);
}

QString OpenFilesDialog::filePathRemote() const
{
    return lineEditRemoteFile->text();
}

void OpenFilesDialog::setFilePathRemote(const QString &newFilePathRemote)
{
    lineEditRemoteFile->setText(newFilePathRemote);
}

QString OpenFilesDialog::filePathBase() const
{
    return lineEditBaseFile->text();
}

void OpenFilesDialog::setFilePathBase(const QString &newFilePathBase)
{
    lineEditBaseFile->setText(newFilePathBase);
}

void OpenFilesDialog::on_toolButtonSelectLocalFile_clicked()
{
    auto fileName = QFileDialog::getOpenFileName(this, i18n("Select local file"));
    if (!fileName.isEmpty())
        lineEditLocalFile->setText(fileName);
}


void OpenFilesDialog::on_toolButtonSelectRemoteFile_clicked()
{
    auto fileName = QFileDialog::getOpenFileName(this, i18n("Select remote file"));
    if (!fileName.isEmpty())
        lineEditRemoteFile->setText(fileName);
}


void OpenFilesDialog::on_toolButtonSelectBaseFile_clicked()
{
    auto fileName = QFileDialog::getOpenFileName(this, i18n("Select base file"));
    if (!fileName.isEmpty())
        lineEditBaseFile->setText(fileName);
}

