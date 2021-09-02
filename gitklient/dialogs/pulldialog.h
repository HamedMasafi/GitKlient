#ifndef PULLDIALOG_H
#define PULLDIALOG_H

#include "ui_pulldialog.h"

namespace Git {
class Manager;
}
class PullDialog : public QDialog, private Ui::PullDialog
{
    Q_OBJECT

public:
    explicit PullDialog(QWidget *parent = nullptr, Git::Manager *git = nullptr);
};

#endif // PULLDIALOG_H
