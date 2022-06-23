#ifndef REMOTESWIDGET_H
#define REMOTESWIDGET_H

#include "ui_remoteswidget.h"
#include "widgetbase.h"

class RemotesActions;
namespace Git {
class RemotesModel;
class Manager;
}
class RemotesWidget : public WidgetBase, private Ui::RemotesWidget
{
    Q_OBJECT
    Git::RemotesModel *_model;
    RemotesActions *_actions;

public:
    explicit RemotesWidget(QWidget *parent = nullptr);
    explicit RemotesWidget(Git::Manager *git, AppWindow *parent = nullptr);


    void saveState(QSettings &settings) const override;
    void restoreState(QSettings &settings) override;


private slots:
    void on_listView_itemActivated(const QModelIndex &index);
    void on_listView_customContextMenuRequested(const QPoint &pos);

private:
    void init(Git::Manager *git);
};

#endif // REMOTESWIDGET_H
