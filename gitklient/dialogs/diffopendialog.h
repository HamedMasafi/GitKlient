#ifndef DIFFOPENDIALOG_H
#define DIFFOPENDIALOG_H

#include "ui_diffopendialog.h"

class DiffOpenDialog : public QDialog, private Ui::DiffOpenDialog
{
    Q_OBJECT

public:
    enum Mode {
        Files,
        Dirs
    };
    explicit DiffOpenDialog(QWidget *parent = nullptr);

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
