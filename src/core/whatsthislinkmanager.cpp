#include "whatsthislinkmanager.h"

#include <QDesktopServices>
#include <QDialog>
#include <QWhatsThisClickedEvent>

WhatsThisLinkManager::WhatsThisLinkManager(QObject *parent) : QObject{parent} {}

WhatsThisLinkManager *WhatsThisLinkManager::instance()
{
    static WhatsThisLinkManager *instance = new WhatsThisLinkManager;
    return instance;
}

void WhatsThisLinkManager::install(QDialog *dialog)
{
    dialog->installEventFilter(this);
}

bool WhatsThisLinkManager::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::WhatsThisClicked) {
        event->accept();
        QWhatsThisClickedEvent *whatsThisEvent = dynamic_cast<QWhatsThisClickedEvent *>(event);
        QDesktopServices::openUrl(whatsThisEvent->href());
        return true;
    }
    return QObject::eventFilter(watched, event);
}
