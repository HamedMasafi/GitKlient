#ifndef WIDGETBASE_H
#define WIDGETBASE_H

#include <QtWidgets/QWidget>

#include <QSettings>

class QSplitter;
class QTreeView;
class AppWindow;

namespace Git {
class Manager;
}
class WidgetBase : public QWidget
{
    Q_OBJECT

protected:
    Git::Manager *_git{nullptr};
    AppWindow *_parent{nullptr};

public:
    explicit WidgetBase(QWidget *parent = nullptr);
    explicit WidgetBase(Git::Manager *git, AppWindow *parent = nullptr);
    Git::Manager *git() const;
    void setGit(Git::Manager *newGit);

    virtual void reload();

    int exec(QWidget *parent = nullptr);

    virtual void saveState(QSettings &settings) const;
    virtual void restoreState(QSettings &settings);

    void save(QSettings &settings, QSplitter *splitter) const;
    void restore(QSettings &settings, QSplitter *splitter);

    void save(QSettings &settings, QTreeView *treeView) const;
    void restore(QSettings &settings, QTreeView *treeView);

private slots:
    void git_pathChanged();
    QString stateName(QWidget *w) const;
};

#endif // WIDGETBASE_H
