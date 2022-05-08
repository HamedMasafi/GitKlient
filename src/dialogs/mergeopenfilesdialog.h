#ifndef OPENFILESDIALOG_H
#define OPENFILESDIALOG_H

#include "../core/appdialog.h"
#include "ui_mergeopenfilesdialog.h"

class MergeOpenFilesDialog : public AppDialog, private Ui::MergeOpenFilesDialog
{
    Q_OBJECT

public:
    explicit MergeOpenFilesDialog(QWidget *parent = nullptr);

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
