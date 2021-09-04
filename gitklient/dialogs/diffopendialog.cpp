#include "diffopendialog.h"

#include <QFileDialog>
#include <KLocalizedString>

DiffOpenDialog::DiffOpenDialog(QWidget *parent) :
      QDialog(parent)
{
    setupUi(this);
    radioButtonCompareFiles->setChecked(true);
    widgetSelectDirectories->hide();
}

QString DiffOpenDialog::oldFile() const
{
    return lineEditOldFile->text();
}

QString DiffOpenDialog::newFile() const
{
    return lineEditNewFile->text();
}

QString DiffOpenDialog::oldDir() const
{
    return lineEditOldDirectory->text();
}

QString DiffOpenDialog::newDir() const
{
    return lineEditNewDirectory->text();
}

DiffOpenDialog::Mode DiffOpenDialog::mode() const
{
    return radioButtonCompareFiles->isChecked() ? Files : Dirs;
}

void DiffOpenDialog::on_toolButtonBrowseNewFile_clicked()
{
    auto f = QFileDialog::getOpenFileName(this, i18n("Select new file"));
    if (!f.isEmpty())
        lineEditNewFile->setText(f);
}

void DiffOpenDialog::on_toolButtonBrowseOldFile_clicked()
{
    auto f = QFileDialog::getOpenFileName(this, i18n("Select old file"));
    if (!f.isEmpty())
        lineEditOldFile->setText(f);
}

void DiffOpenDialog::on_toolButtonBrowseNewDirectory_clicked()
{
    auto f = QFileDialog::getExistingDirectory(this, i18n("Select new directory"));
    if (!f.isEmpty())
        lineEditNewDirectory->setText(f);
}

void DiffOpenDialog::on_toolButtonBrowseOldDirectory_clicked()
{
    auto f = QFileDialog::getExistingDirectory(this, i18n("Select old directory"));
    if (!f.isEmpty())
        lineEditOldDirectory->setText(f);
}
