#ifndef CLONEDIALOG_H
#define CLONEDIALOG_H

#include "ui_clonedialog.h"

class CloneDialog : public QDialog, private Ui::CloneDialog
{
    Q_OBJECT

public:
    explicit CloneDialog(QWidget *parent = nullptr);

private slots:
    void on_lineEditUrl_textChanged(QString text);
    void on_toolButtonBrowseLocalPath_clicked();
};

#endif // CLONEDIALOG_H
