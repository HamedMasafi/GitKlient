#include "clonedialog.h"

#include <QFileDialog>
#include <KLocalizedString>
#include <QStandardPaths>
#include <QSettings>

CloneDialog::CloneDialog(QWidget *parent) :
      AppDialog(parent)
{
    setupUi(this);
    _fixedPath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);

    QSettings s;
    lineEditUrl->setText(s.value("lastClonedRepo").toString());
}

CloneDialog::~CloneDialog()
{
    QSettings s;
    s.setValue("lastClonedRepo", lineEditUrl->text());
}

Git::CloneCommand *CloneDialog::command()
{
    auto cmd = new Git::CloneCommand(this);

    cmd->setRepoUrl(lineEditUrl->text());
    cmd->setLocalPath(lineEditPath->text());
    if (checkBoxBranch->isChecked())
        cmd->setBranch(lineEditBranch->text());
    if(checkBoxDepth->isChecked())
        cmd->setDepth(spinBoxDepth->value());
    cmd->setRecursive(checkBoxRecursive->isChecked());
    return cmd;
}

void CloneDialog::setLocalPath(const QString &path)
{
    lineEditPath->setText(path);
}

void CloneDialog::on_lineEditUrl_textChanged(QString text)
{
    auto parts = text.split("/");
    if (parts.size()) {
        auto local = parts.last();
        if (local.endsWith(".git", Qt::CaseInsensitive))
            local = local.mid(0, local.size() - 4);

        lineEditPath->setText(_fixedPath + "/" + local);
    }
}

void CloneDialog::on_toolButtonBrowseLocalPath_clicked()
{
    auto localPath = QFileDialog::getExistingDirectory(this, i18n("Local path"));
    if (!localPath.isEmpty())
        lineEditPath->setText(localPath);
}

void CloneDialog::on_buttonBox_accepted()
{
    accept();
}
