#ifndef OPENFILESDIALOG_H
#define OPENFILESDIALOG_H

#include "ui_openfilesdialog.h"

class OpenFilesDialog : public QDialog, private Ui::OpenFilesDialog
{
    Q_OBJECT

public:
    explicit OpenFilesDialog(QWidget *parent = nullptr);

    QString filePathLocal() const;
    void setFilePathLocal(const QString &newFilePathLocal);

    QString filePathRemote() const;
    void setFilePathRemote(const QString &newFilePathRemote);

    QString filePathBase() const;
    void setFilePathBase(const QString &newFilePathBase);

private slots:
    void on_toolButtonSelectLocalFile_clicked();
    void on_toolButtonSelectRemoteFile_clicked();
    void on_toolButtonSelectBaseFile_clicked();
};

#endif // OPENFILESDIALOG_H
