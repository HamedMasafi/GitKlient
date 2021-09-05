#include "clonedialog.h"

#include <QFileDialog>

CloneDialog::CloneDialog(QWidget *parent) :
      QDialog(parent)
{
    setupUi(this);
}

void CloneDialog::on_lineEditUrl_textChanged(QString text)
{
    auto parts = text.split("/");
    if (parts.size()) {
        auto local = parts.last();
        if (local.toLower().endsWith(".git"))
            local = local.mid(0, local.size() - 4);

        lineEditPath->setText("/fixed/" + local);
    }
}

void CloneDialog::on_toolButtonBrowseLocalPath_clicked()
{
    auto localPath = QFileDialog::getExistingDirectory(this, i18n("Local path"));
    if (!localPath.isEmpty())
        lineEditPath->setText(localPath);
}
