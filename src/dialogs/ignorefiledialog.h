#ifndef IGNOREFILEDIALOG_H
#define IGNOREFILEDIALOG_H

#include "ui_ignorefiledialog.h"

#include "core/appdialog.h"

class IgnoreFileDialog : public AppDialog, private Ui::IgnoreFileDialog
{
    Q_OBJECT

public:
    explicit IgnoreFileDialog(Git::Manager *git, const QString &filePath, QWidget *parent = nullptr);

private slots:
    void generateIgnorePattern();
    void on_buttonBox_accepted();

private:
    bool _isIgnoredAlready{false};
    QString _path, _name, _ext;
    QString getIgnoreFile();
};

#endif // IGNOREFILEDIALOG_H
