#ifndef STASHESWIDGET_H
#define STASHESWIDGET_H

#include "ui_stasheswidget.h"
#include "widgetbase.h"
#include "git/stash.h"

namespace Git {
class StashesCache;
}
class QStandardItemModel;
class StashActions;
class StashesWidget : public WidgetBase, private Ui::StashesWidget
{
    Q_OBJECT
    StashActions *_actions;
    Git::StashesCache *_model;

public:
    explicit StashesWidget(QWidget *parent = nullptr);
    StashesWidget(Git::Manager *git, GitKlientWindow *parent = nullptr);

    void reload() override;
private slots:
    void on_treeView_customContextMenuRequested(const QPoint &pos);

    // WidgetBase interface
public:
    void saveState(QSettings &settings) const override;
    void restoreState(QSettings &settings) override;
private:
    void init(Git::Manager *git);
};

#endif // STASHESWIDGET_H
