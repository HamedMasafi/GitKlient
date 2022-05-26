#include "diffopendialog.h"

#include <QFileDialog>
#include <KLocalizedString>
#include <QSettings>

DiffOpenDialog::DiffOpenDialog(QWidget *parent) :
      AppDialog(parent)
{
    setupUi(this);
    radioButtonCompareFiles->setChecked(true);
    widgetSelectDirectories->hide();

    QSettings s;
    s.beginGroup("diff");
    lineEditOldFile->setText(s.value("oldFile").toString());
    lineEditNewFile->setText(s.value("newFile").toString());
    lineEditOldDirectory->setText(s.value("oldDir").toString());
    lineEditNewDirectory->setText(s.value("newDir").toString());
    s.endGroup();
}

DiffOpenDialog::~DiffOpenDialog()
{
    QSettings s;
    s.beginGroup("diff");
    s.setValue("oldFile", lineEditOldFile->text());
    s.setValue("newFile", lineEditNewFile->text());
    s.setValue("oldDir", lineEditOldDirectory->text());
    s.setValue("newDir", lineEditNewDirectory->text());
    s.endGroup();
    s.sync();
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
