#ifndef FETCHDIALOG_H
#define FETCHDIALOG_H

#include "../core/appdialog.h"
#include "ui_fetchdialog.h"

namespace Git {
class Manager;
}
class FetchDialog : public AppDialog, private Ui::FetchDialog
{
    Q_OBJECT

public:
    explicit FetchDialog(Git::Manager *git, QWidget *parent = nullptr);

    void setBranch(const QString &branch);

private slots:
    void on_buttonBox_accepted();
};

#endif // FETCHDIALOG_H
