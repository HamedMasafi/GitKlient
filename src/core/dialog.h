#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Git {
class Manager;
}

class Dialog : public QDialog
{
    Q_OBJECT

protected:
    Git::Manager *_git;

public:
    Dialog(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    Dialog(Git::Manager *git, QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());

    bool event(QEvent *event) override;
};

#endif // DIALOG_H
