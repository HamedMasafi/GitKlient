#ifndef WIDGETBASE_H
#define WIDGETBASE_H

#include <QtWidgets/QWidget>
namespace Git {
class Manager;
}
class WidgetBase : public QWidget
{
    Q_OBJECT

    Git::Manager *_git{nullptr};

public:
    explicit WidgetBase(QWidget *parent = nullptr);
    explicit WidgetBase(Git::Manager *git, QWidget *parent = nullptr);
    Git::Manager *git() const;
    void setGit(Git::Manager *newGit);

    virtual void reload();

    int exec(QWidget *parent = nullptr);

private slots:
    void git_pathChanged();
};

#endif // WIDGETBASE_H
