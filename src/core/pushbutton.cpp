#include "pushbutton.h"

#include <QAction>

PushButton::PushButton(QWidget *parent) : QPushButton(parent)
{

}

QAction *PushButton::action() const
{
    return _action;
}

void PushButton::setAction(QAction *newAction)
{

    if (_action && _action != newAction) {
        disconnect(_action, &QAction::changed,
                   this, &PushButton::updateButtonStatusFromAction);
        disconnect(this, &PushButton::clicked,
                   _action, &QAction::trigger);
    }
    _action = newAction;

    connect(_action, &QAction::changed,
            this, &PushButton::updateButtonStatusFromAction);
    connect(this, &PushButton::clicked,
            _action, &QAction::trigger);

    updateButtonStatusFromAction();
}

void PushButton::updateButtonStatusFromAction() {

    if (!_action)
        return;
    setText(_action->text());
    setStatusTip(_action->statusTip());
    setToolTip(_action->toolTip());
    setIcon(_action->icon());
    setEnabled(_action->isEnabled());
    setCheckable(_action->isCheckable());
    setChecked(_action->isChecked());
}
