#ifndef SUBMODULESWIDGET_H
#define SUBMODULESWIDGET_H

#include "ui_submoduleswidget.h"
#include "widgetbase.h"

class QTreeWidgetItem;
class SubmoduleActions;
class SubmodulesWidget : public WidgetBase, private Ui::SubmodulesWidget
{
    Q_OBJECT
    SubmoduleActions *_actions;

public:
    explicit SubmodulesWidget(QWidget *parent = nullptr);
    explicit SubmodulesWidget(Git::Manager *git, GitKlientWindow *parent = nullptr);


    void saveState(QSettings &settings) const override;
    void restoreState(QSettings &settings) override;

    void reload() override;
private slots:
    void on_treeWidget_customContextMenuRequested(const QPoint &pos);
    void on_treeWidget_itemActivated(QTreeWidgetItem *item, int);
};

#endif // SUBMODULESWIDGET_H
