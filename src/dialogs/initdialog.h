#ifndef INITDIALOG_H
#define INITDIALOG_H

#include "ui_initdialog.h"

#include "core/dialog.h"

class InitDialog : public Dialog, private Ui::InitDialog
{
    Q_OBJECT

public:
    explicit InitDialog(Git::Manager *git, QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    QString path() const;
};

#endif // INITDIALOG_H
