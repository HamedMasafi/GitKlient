#ifndef CLONEDIALOG_H
#define CLONEDIALOG_H

#include "../core/appdialog.h"
#include "ui_clonedialog.h"

#include <git/commands/commandclone.h>

class CloneDialog : public AppDialog, private Ui::CloneDialog
{
    Q_OBJECT
    QString _fixedPath;

public:
    explicit CloneDialog(QWidget *parent = nullptr);
    ~CloneDialog();

    Git::CloneCommand *command();

    void setLocalPath(const QString &path);

private slots:
    void on_lineEditUrl_textChanged(QString text);
    void on_toolButtonBrowseLocalPath_clicked();
    void on_buttonBox_accepted();
};

#endif // CLONEDIALOG_H
