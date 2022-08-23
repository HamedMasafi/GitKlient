#ifndef DIFFOPENDIALOG_H
#define DIFFOPENDIALOG_H

#include "../core/appdialog.h"
#include "ui_diffopendialog.h"

class DiffOpenDialog : public AppDialog, private Ui::DiffOpenDialog
{
    Q_OBJECT

public:
    enum Mode {
        Files,
        Dirs
    };
    explicit DiffOpenDialog(QWidget *parent = nullptr);
    ~DiffOpenDialog() override;

    QString oldFile() const;
    QString newFile() const;
    QString oldDir() const;
    QString newDir() const;
    Mode mode() const;

private slots:
    void on_toolButtonBrowseNewFile_clicked();
    void on_toolButtonBrowseOldFile_clicked();
    void on_toolButtonBrowseNewDirectory_clicked();
    void on_toolButtonBrowseOldDirectory_clicked();
};

#endif // DIFFOPENDIALOG_H
