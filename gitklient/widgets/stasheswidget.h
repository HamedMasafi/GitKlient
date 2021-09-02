#ifndef STASHESWIDGET_H
#define STASHESWIDGET_H

#include "ui_stasheswidget.h"
#include "widgetbase.h"

class StashesWidget : public WidgetBase, private Ui::StashesWidget
{
    Q_OBJECT

public:
    explicit StashesWidget(QWidget *parent = nullptr);
    StashesWidget(Git::Manager *git, QWidget *parent = nullptr);

    void reload() override;
private slots:
    void on_pushButtonCreateNew_clicked();
    void on_pushButtonApply_clicked();
    void on_pushButtonRemoveSelected_clicked();
    void on_listWidget_itemActivated(QListWidgetItem *item);
};

#endif // STASHESWIDGET_H
