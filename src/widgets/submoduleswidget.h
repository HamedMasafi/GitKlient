#ifndef SUBMODULESWIDGET_H
#define SUBMODULESWIDGET_H

#include "ui_submoduleswidget.h"
#include "widgetbase.h"

class SubmodulesWidget : public WidgetBase, private Ui::SubmodulesWidget
{
    Q_OBJECT

public:
    explicit SubmodulesWidget(QWidget *parent = nullptr);
    explicit SubmodulesWidget(Git::Manager *git, QWidget *parent = nullptr);

    void reload();
private slots:
    void on_pushButtonAddNew_clicked();
};

#endif // SUBMODULESWIDGET_H
