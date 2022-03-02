#ifndef TAGSWIDGET_H
#define TAGSWIDGET_H

#include "ui_tagswidget.h"
#include "widgetbase.h"

class TagsWidget : public WidgetBase, private Ui::TagsWidget
{
    Q_OBJECT

public:
    explicit TagsWidget(QWidget *parent = nullptr);
    explicit TagsWidget(Git::Manager *git, GitKlientWindow *parent = nullptr);
    void reload() override;
private slots:
    void on_pushButtonAddTag_clicked();
};

#endif // TAGSWIDGET_H
