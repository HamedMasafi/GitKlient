#ifndef WIDGETBASE_H
#define WIDGETBASE_H

#include <QtWidgets/QWidget>

#include <QSettings>

class QSplitter;
class QTreeView;

namespace Git {
class Manager;
}
class WidgetBase : public QWidget
{
    Q_OBJECT

    Git::Manager *_git{nullptr};

public:
    explicit WidgetBase(QWidget *parent = nullptr);
    WidgetBase(Git::Manager *git, QWidget *parent = nullptr);
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
