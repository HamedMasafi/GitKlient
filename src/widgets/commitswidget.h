#ifndef COMMITSWIDGET_H
#define COMMITSWIDGET_H

#include "ui_commitswidget.h"
#include "widgetbase.h"

class QMenu;
class TreeModel;
class CommitsWidget : public WidgetBase, private Ui::CommitsWidget
{
    Q_OBJECT

    TreeModel *_repoModel;
    QMenu *_branchesMenu;
    QString _mainBranch;

public:
    explicit CommitsWidget(QWidget *parent = nullptr);
    explicit CommitsWidget(Git::Manager *git, QWidget *parent = nullptr);
    void reload() override;

private slots:
    void on_treeViewRepo_itemActivated(const QModelIndex &index);
    void on_treeViewRepo_customContextMenuRequested(const QPoint &pos);

    void on_actionBrowse_triggered();
    void on_actionDiffWithMain_triggered();

private:
    void init();
};

#endif // COMMITSWIDGET_H
