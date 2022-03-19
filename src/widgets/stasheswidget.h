#ifndef STASHESWIDGET_H
#define STASHESWIDGET_H

#include "ui_stasheswidget.h"
#include "widgetbase.h"
#include "git/stash.h"

class QStandardItemModel;
class StashActions;
class StashesWidget : public WidgetBase, private Ui::StashesWidget
{
    Q_OBJECT
    QList<Git::Stash> _stashes;
    StashActions *_actions;
    QStandardItemModel *model;

public:
    explicit StashesWidget(QWidget *parent = nullptr);
    StashesWidget(Git::Manager *git, GitKlientWindow *parent = nullptr);

    void reload() override;
private slots:
    void on_pushButtonCreateNew_clicked();
    void on_pushButtonApply_clicked();
    void on_pushButtonRemoveSelected_clicked();
    void on_listWidget_itemActivated(QListWidgetItem *item);
    void on_listWidget_customContextMenuRequested(const QPoint &pos);
    void on_treeView_customContextMenuRequested(const QPoint &pos);

    // WidgetBase interface
public:
    void saveState(QSettings &settings) const override;
    void restoreState(QSettings &settings) override;
};

#endif // STASHESWIDGET_H
