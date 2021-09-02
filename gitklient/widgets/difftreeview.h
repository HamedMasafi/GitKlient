#ifndef DIFFTREEVIEW_H
#define DIFFTREEVIEW_H

#include "ui_difftreeview.h"

class QSortFilterProxyModel;
class DiffTreeModel;
class FilesModel;
class DiffTreeView : public QWidget, private Ui::DiffTreeView
{
    Q_OBJECT

    DiffTreeModel *_diffModel{nullptr};
    QSortFilterProxyModel *_filterModel{nullptr};
    FilesModel *_filesModel;

public:
    explicit DiffTreeView(QWidget *parent = nullptr);

    DiffTreeModel *diffModel() const;
    void setDiffModel(DiffTreeModel *newDiffModel, FilesModel *filesModel);

private slots:
    void on_lineEditFilter_textChanged(QString text);
    void on_treeView_clicked(const QModelIndex &index);
    void on_listView_clicked(const QModelIndex &index);

signals:
    void fileSelected(const QString &file);
};

#endif // DIFFTREEVIEW_H
