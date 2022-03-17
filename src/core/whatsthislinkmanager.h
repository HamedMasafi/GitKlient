#ifndef WHATSTHISLINKMANAGER_H
#define WHATSTHISLINKMANAGER_H

#include <QObject>

class QDialog;
class WhatsThisLinkManager : public QObject
{
    Q_OBJECT
public:
    explicit WhatsThisLinkManager(QObject *parent = nullptr);

    static WhatsThisLinkManager *instance();
    void install(QDialog *dialog);
    bool eventFilter(QObject *watched, QEvent *event) override;
};

#endif // WHATSTHISLINKMANAGER_H
