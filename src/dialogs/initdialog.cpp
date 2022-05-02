#include "initdialog.h"

InitDialog::InitDialog(Git::Manager *git, QWidget *parent, Qt::WindowFlags f)
    : Dialog(git, parent, f)
{
    setupUi(this);
}

QString InitDialog::path() const
{
    return lineEditPath->text();
}
