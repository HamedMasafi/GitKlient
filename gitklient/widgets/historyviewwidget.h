#ifndef HISTORYVIEWWIDGET_H
#define HISTORYVIEWWIDGET_H

#include "ui_historyviewwidget.h"
#include "widgetbase.h"

class HistoryModel;
class GraphPainter;
class HistoryViewWidget : public WidgetBase, private Ui::HistoryViewWidget
{
    Q_OBJECT
    HistoryModel *_historyModel;
    QMenu *_commitMenu;
    GraphPainter *_graphPainter;

public:
    explicit HistoryViewWidget(QWidget *parent = nullptr);
    explicit HistoryViewWidget(Git::Manager *git, QWidget *parent = nullptr);
    void setBranch(const QString &branchName);

private slots:
    void browseCommit();
    void on_treeViewHistory_activated(const QModelIndex &index);
    void on_textBrowser_hashClicked(const QString &hash);
    void on_textBrowser_fileClicked(const QString &file);
    void on_treeViewHistory_customContextMenuRequested(const QPoint &pos);

    void on_treeViewHistory_clicked(const QModelIndex &index);
    void git_pathChanged();
};

#endif // HISTORYVIEWWIDGET_H
