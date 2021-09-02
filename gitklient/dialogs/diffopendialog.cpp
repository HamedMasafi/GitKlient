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
