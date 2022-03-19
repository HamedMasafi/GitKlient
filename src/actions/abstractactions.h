#ifndef ABSTRACTACTIONS_H
#define ABSTRACTACTIONS_H

#include <QObject>

#define ADD_ACTION(name, text, slot)                                                               \
    _##name = new QAction(this);                                                                   \
    _##name->setText(i18n(text));                                                                     \
    connect(_##name, &QAction::triggered, this, slot);                                                \
    _menu->addAction(_##name);

#define DEFINE_ACTION(name)                                                                        \
private:                                                                                           \
    QAction *_##name;                                                                              \
                                                                                                   \
public:                                                                                            \
    QAction *name() const { return _##name; }

namespace Git {
class Manager;
};

class QWidget;
class QMenu;
class QAction;
class AbstractActions : public QObject
{
    Q_OBJECT

protected:
    Git::Manager *_git;
    QMenu *_menu;
    QWidget *_parent;

public:
    explicit AbstractActions(Git::Manager *git, QWidget *parent);
    void popup();
    void popup(const QPoint &pos);
signals:
};

#endif // ABSTRACTACTIONS_H
