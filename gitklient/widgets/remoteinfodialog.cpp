#include "remoteinfodialog.h"

RemoteInfoDialog::RemoteInfoDialog(QWidget *parent) :
      QDialog(parent)
{
    setupUi(this);
}

QString RemoteInfoDialog::remoteName() const
{
    return lineEditName->text();
}

QString RemoteInfoDialog::remoteUrl() const
{
    return lineEditUrl->text();
}
