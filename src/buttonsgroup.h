#ifndef BUTTONSGROUP_H
#define BUTTONSGROUP_H

#include <QtCore/QObject>

#include <QList>
#include <QMap>

class QAbstractButton;
class ButtonsGroup : public QObject
{
    Q_OBJECT
    QList<QAbstractButton*> _buttons;
    QMap<QAbstractButton*, QObject*> _data;

public:
    explicit ButtonsGroup(QObject *parent = nullptr);
    void addButton(QAbstractButton *btn, QObject *data = nullptr);
    QAbstractButton *at(int i) const;
    void simulateClickOn(int index);

private slots:
    void buttonClicked();

signals:
    void clicked(int index);
};

#endif // BUTTONSGROUP_H
