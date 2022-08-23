#ifndef MULTIPAGEWIDGET_H
#define MULTIPAGEWIDGET_H

#include "ui_multipagewidget.h"

#include <QIcon>
#include <QMap>

namespace Git {
class Manager;
}
class QActionGroup;
class QToolButton;
class WidgetBase;
class MultiPageWidget : public QWidget, private Ui::MultiPageWidget
{
    Q_OBJECT

    QActionGroup *actionGroup;
    Git::Manager *_defaultGitManager{};
public:
    explicit MultiPageWidget(QWidget *parent = nullptr);
    void addPage(const QString &title, const QIcon &icon, WidgetBase *widget);
    void addPage(WidgetBase *widget, QAction *action);
    template<class T>
    void addPage() {
        addPage(new T(_defaultGitManager, this));
    }
    void setCurrentIndex(int index);
    QList<QAction *> actions() const;
    Git::Manager *defaultGitManager() const;
    void setDefaultGitManager(Git::Manager *newDefaultGitManager);

    int count() const;
private slots:
    void on_actionGroup_triggered(QAction *action);
};

#endif // MULTIPAGEWIDGET_H
