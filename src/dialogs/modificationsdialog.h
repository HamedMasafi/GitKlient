#ifndef MODIFICATIONSDIALOG_H
#define MODIFICATIONSDIALOG_H

#include "ui_modificationsdialog.h"

class ModificationsDialog : public QDialog, private Ui::ModificationsDialog
{
    Q_OBJECT

public:
    explicit ModificationsDialog(QWidget *parent = nullptr);
};

#endif // MODIFICATIONSDIALOG_H
