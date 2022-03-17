#include "mergedialog.h"

#include "core/whatsthislinkmanager.h"

#include <QWhatsThisClickedEvent>
#include <QDesktopServices>

MergeDialog::MergeDialog(QWidget *parent) :
      QDialog(parent)
{
    setupUi(this);
    WhatsThisLinkManager::instance()->install(this);
}

MergeDialog::MergeDialog(const QString &sourceBranch, QWidget *parent):
      QDialog(parent), _sourceBranch(sourceBranch)
{
    setupUi(this);
    WhatsThisLinkManager::instance()->install(this);
}
