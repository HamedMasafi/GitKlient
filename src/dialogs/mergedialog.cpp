#include "mergedialog.h"

#include <QWhatsThisClickedEvent>
#include <QDesktopServices>

MergeDialog::MergeDialog(QWidget *parent) :
      QDialog(parent)
{
    setupUi(this);
}

MergeDialog::MergeDialog(const QString &sourceBranch, QWidget *parent):
      QDialog(parent), _sourceBranch(sourceBranch)
{
    setupUi(this);
}

bool  MergeDialog::event(QEvent* ev)
{
    if (ev->type() == QEvent::WhatsThisClicked)
    {
        ev->accept();
        QWhatsThisClickedEvent* whatsThisEvent = dynamic_cast<QWhatsThisClickedEvent*>(ev);
//        assert(whatsThisEvent);
        QDesktopServices::openUrl(whatsThisEvent->href());
        return true;
    }
    return QDialog::event(ev);
}
