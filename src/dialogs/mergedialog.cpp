#include "mergedialog.h"

#include <QWhatsThisClickedEvent>
#include <QDesktopServices>

MergeDialog::MergeDialog(QWidget *parent) :
      Dialog(parent)
{
    setupUi(this);
}

MergeDialog::MergeDialog(const QString &sourceBranch, QWidget *parent):
      Dialog(parent), _sourceBranch(sourceBranch)
{
    setupUi(this);
}
