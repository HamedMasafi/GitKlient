#include "buttonsgroup.h"

#include <QAbstractButton>

ButtonsGroup::ButtonsGroup(QObject *parent) : QObject(parent) {}

void ButtonsGroup::addButton(QAbstractButton *btn, QObject *data)
{
    btn->setCheckable(true);
    btn->setChecked(!_buttons.size());
    connect(btn, &QAbstractButton::clicked, this, &ButtonsGroup::buttonClicked);
    if (data)
        _data.insert(btn, data);
    _buttons.append(btn);
}

QAbstractButton *ButtonsGroup::at(int i) const
{
    return _buttons.at(i);
}

void ButtonsGroup::simulateClickOn(int index)
{
    emit clicked(index);
}

void ButtonsGroup::buttonClicked()
{
    auto btn = qobject_cast<QAbstractButton *>(sender());
    if (!btn)
        return;

    for (auto &b : _buttons)
        b->setChecked(b == btn);

    auto index = _buttons.indexOf(btn);
    emit clicked(index);
}
