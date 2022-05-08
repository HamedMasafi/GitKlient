#include "widgetbase.h"

#include "git/gitmanager.h"

#include <QDialog>
#include <QPushButton>
#include <QSplitter>
#include <QHeaderView>
#include <QTreeView>
#include <QVBoxLayout>
#include "appwindow.h"

WidgetBase::WidgetBase(QWidget *parent) : QWidget(parent)
{
    _git = Git::Manager::instance();
    connect(_git, &Git::Manager::pathChanged, this, &WidgetBase::git_pathChanged);
}

WidgetBase::WidgetBase(Git::Manager *git, AppWindow *parent) : QWidget(parent),
      _git(git), _parent{parent}
{
    if (!_git)
        _git = Git::Manager::instance();
    connect(_git, &Git::Manager::pathChanged, this, &WidgetBase::git_pathChanged);
}

Git::Manager *WidgetBase::git() const
{
    return _git;
}

void WidgetBase::setGit(Git::Manager *newGit)
{
    _git = newGit;
}

void WidgetBase::reload()
{

}

int WidgetBase::exec(QWidget *parent)
{
    auto oldParent = this->parentWidget();
    QDialog d(parent);
    QVBoxLayout layout(&d);
    this->setParent(&d);
    layout.addWidget(this);
//    layout.addWidget(new QPushButton(&d));
    int ret = d.exec();
    setParent(oldParent);
    return ret;

}

void WidgetBase::saveState(QSettings &settings) const
{
    Q_UNUSED(settings)
}

void WidgetBase::restoreState(QSettings &settings)
{
    Q_UNUSED(settings)
}

void WidgetBase::save(QSettings &settings, QSplitter *splitter) const
{
    settings.setValue(stateName(splitter), splitter->saveState());
}

void WidgetBase::restore(QSettings &settings, QSplitter *splitter)
{
    splitter->restoreState(settings.value(stateName(splitter)).toByteArray());
}

void WidgetBase::save(QSettings &settings, QTreeView *treeView) const
{
    settings.setValue(stateName(treeView), treeView->header()->saveState());
}

void WidgetBase::restore(QSettings &settings, QTreeView *treeView)
{
    treeView->header()->restoreState(settings.value(stateName(treeView)).toByteArray());
}

void WidgetBase::git_pathChanged()
{
    reload();
}

QString WidgetBase::stateName(QWidget *w) const
{
    return QStringLiteral("%1_%2_state").arg(metaObject()->className(), w->objectName());
}
