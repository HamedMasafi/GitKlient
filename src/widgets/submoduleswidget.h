#ifndef SUBMODULESWIDGET_H
#define SUBMODULESWIDGET_H

#include "ui_submoduleswidget.h"
#include "widgetbase.h"

class QTreeWidgetItem;
class SubmoduleActions;

namespace Git {
class SubmodulesModel;
}

class SubmodulesWidget : public WidgetBase, private Ui::SubmodulesWidget
{
    Q_OBJECT
    SubmoduleActions *_actions;
    Git::SubmodulesModel *_model;

public:
    explicit SubmodulesWidget(QWidget *parent = nullptr);
    explicit SubmodulesWidget(Git::Manager *git, AppWindow *parent = nullptr);


    void saveState(QSettings &settings) const override;
    void restoreState(QSettings &settings) override;

    void reload() override;
private slots:
    void on_treeView_customContextMenuRequested(const QPoint &pos);
    void on_treeView_activated(const QModelIndex &index);
};

#endif // SUBMODULESWIDGET_H
