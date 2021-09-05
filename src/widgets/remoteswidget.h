#ifndef REMOTESWIDGET_H
#define REMOTESWIDGET_H

#include "ui_remoteswidget.h"
#include "widgetbase.h"

class RemotesWidget : public WidgetBase, private Ui::RemotesWidget
{
    Q_OBJECT

public:
    explicit RemotesWidget(QWidget *parent = nullptr);
    explicit RemotesWidget(Git::Manager *git, QWidget *parent = nullptr);
    void reload() override;

private slots:
    void on_toolButtonAdd_clicked();
    void on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);
    void on_toolButtonRemove_clicked();
};

#endif // REMOTESWIDGET_H
