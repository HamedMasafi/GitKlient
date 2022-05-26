#ifndef REMOTEINFODIALOG_H
#define REMOTEINFODIALOG_H

#include "ui_remoteinfodialog.h"
namespace Git {
class CommandAddRemote;
}
class RemoteInfoDialog : public QDialog, private Ui::RemoteInfoDialog
{
    Q_OBJECT

public:
    explicit RemoteInfoDialog(QWidget *parent = nullptr);

    QString remoteName() const;
    QString remoteUrl() const;
    Git::CommandAddRemote *command();
};

#endif // REMOTEINFODIALOG_H
