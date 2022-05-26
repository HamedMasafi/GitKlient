#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Git {
class Manager;
}

class AppDialog : public QDialog
{
    Q_OBJECT

protected:
    Git::Manager *_git;

public:
    AppDialog(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    AppDialog(Git::Manager *git, QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());

    bool event(QEvent *event) override;
};

#endif // DIALOG_H
