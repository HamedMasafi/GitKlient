#include "dialog.h"

#include "git/gitmanager.h"
#include <QEvent>
#include <QWhatsThisClickedEvent>
#include <QDesktopServices>

Dialog::Dialog(QWidget *parent, Qt::WindowFlags f)
    : QDialog(parent, f), _git(Git::Manager::instance())
{}

Dialog::Dialog(Git::Manager *git, QWidget *parent, Qt::WindowFlags f)
    : QDialog(parent, f), _git(git)
{}

bool Dialog::event(QEvent *event)
{
    if (event->type() == QEvent::WhatsThisClicked) {
        event->accept();
        QWhatsThisClickedEvent *whatsThisEvent = dynamic_cast<QWhatsThisClickedEvent *>(event);
        QDesktopServices::openUrl(whatsThisEvent->href());
        return true;
    }
    return QDialog::event(event);
}
