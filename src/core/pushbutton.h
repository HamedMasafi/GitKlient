#ifndef PUSHBUTTON_H
#define PUSHBUTTON_H

#include <QPushButton>

class PushButton : public QPushButton
{
    Q_OBJECT

public:
    explicit PushButton(QWidget *parent = nullptr);
    QAction *action() const;
    void setAction(QAction *newAction);

private:
    QAction *_action{nullptr};

private slots:
    void updateButtonStatusFromAction();
};

#endif // PUSHBUTTON_H
