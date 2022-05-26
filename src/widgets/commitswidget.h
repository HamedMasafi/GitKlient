#ifndef COMMITSWIDGET_H
#define COMMITSWIDGET_H

#include "ui_commitswidget.h"
#include "widgetbase.h"

class BranchActions;
class TreeModel;
class CommitsWidget : public WidgetBase, private Ui::CommitsWidget
{
    Q_OBJECT

    TreeModel *_repoModel;
    BranchActions *_actions;
    QString _mainBranch;

public:
    explicit CommitsWidget(QWidget *parent = nullptr);
    explicit CommitsWidget(Git::Manager *git, AppWindow *parent = nullptr);
    void reload() override;

    void saveState(QSettings &settings) const override;
    void restoreState(QSettings &settings) override;

private slots:
    void on_treeViewRepo_itemActivated(const QModelIndex &index);
    void on_treeViewRepo_customContextMenuRequested(const QPoint &pos);

private:
    void init();
};

#endif // COMMITSWIDGET_H
