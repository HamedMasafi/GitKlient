#ifndef MODIFICATIONSDIALOG_H
#define MODIFICATIONSDIALOG_H

#include "../core/appdialog.h"
#include "ui_modificationsdialog.h"

class ModificationsDialog : public AppDialog, private Ui::ModificationsDialog
{
    Q_OBJECT

public:
    explicit ModificationsDialog(QWidget *parent = nullptr);
};

#endif // MODIFICATIONSDIALOG_H
