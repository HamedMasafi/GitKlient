#ifndef INITDIALOG_H
#define INITDIALOG_H

#include "ui_initdialog.h"

#include "core/appdialog.h"

class InitDialog : public AppDialog, private Ui::InitDialog
{
    Q_OBJECT

public:
    explicit InitDialog(Git::Manager *git, QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    QString path() const;
    void setPath(const QString &path);
};

#endif // INITDIALOG_H
