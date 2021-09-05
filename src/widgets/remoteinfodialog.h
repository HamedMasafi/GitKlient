#ifndef REMOTEINFODIALOG_H
#define REMOTEINFODIALOG_H

#include "ui_remoteinfodialog.h"

class RemoteInfoDialog : public QDialog, private Ui::RemoteInfoDialog
{
    Q_OBJECT

public:
    explicit RemoteInfoDialog(QWidget *parent = nullptr);

    QString remoteName() const;
    QString remoteUrl() const;
};

#endif // REMOTEINFODIALOG_H
