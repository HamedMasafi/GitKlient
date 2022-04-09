#ifndef TAGSWIDGET_H
#define TAGSWIDGET_H

#include "ui_tagswidget.h"
#include "widgetbase.h"

class TagsActions;
class TagsWidget : public WidgetBase, private Ui::TagsWidget
{
    Q_OBJECT
    TagsActions *_actions;

public:
    explicit TagsWidget(QWidget *parent = nullptr);
    explicit TagsWidget(Git::Manager *git, GitKlientWindow *parent = nullptr);
    void reload() override;

private slots:
    void on_pushButtonAddTag_clicked();
    void on_treeViewTags_customContextMenuRequested(const QPoint &pos);

public:
    void saveState(QSettings &settings) const override;
    void restoreState(QSettings &settings) override;
};

#endif // TAGSWIDGET_H
