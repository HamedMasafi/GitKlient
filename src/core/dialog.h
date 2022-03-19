#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());

    bool event(QEvent *event) override;
};

#endif // DIALOG_H
