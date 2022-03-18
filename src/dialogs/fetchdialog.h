#ifndef FETCHDIALOG_H
#define FETCHDIALOG_H

#include "ui_fetchdialog.h"

namespace Git {
class Manager;
}
class FetchDialog : public QDialog, private Ui::FetchDialog
{
    Q_OBJECT

public:
    explicit FetchDialog(Git::Manager *git, QWidget *parent = nullptr);

private slots:
    void on_buttonBox_accepted();
};

#endif // FETCHDIALOG_H
