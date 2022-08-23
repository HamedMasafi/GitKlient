#include "appdialog.h"

#include "git/gitmanager.h"
#include <QEvent>
#include <QWhatsThisClickedEvent>
#include <QDesktopServices>

AppDialog::AppDialog(QWidget *parent, Qt::WindowFlags f)
    : QDialog(parent, f), _git(Git::Manager::instance())
{}

AppDialog::AppDialog(Git::Manager *git, QWidget *parent, Qt::WindowFlags f)
    : QDialog(parent, f), _git(git)
{}

bool AppDialog::event(QEvent *event)
{
    if (event->type() == QEvent::WhatsThisClicked) {
        event->accept();
        auto whatsThisEvent = dynamic_cast<QWhatsThisClickedEvent *>(event);
        QDesktopServices::openUrl(whatsThisEvent->href());
        return true;
    }
    return QDialog::event(event);
}
