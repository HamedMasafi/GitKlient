#ifndef DIFFTREEVIEW_H
#define DIFFTREEVIEW_H

#include "ui_difftreeview.h"

class QSortFilterProxyModel;
class DiffTreeModel;
class FilesModel;
class QStringListModel;
class DiffTreeView : public QWidget, private Ui::DiffTreeView
{
    Q_OBJECT

    DiffTreeModel *_diffModel{nullptr};
    QSortFilterProxyModel *_filterModel{nullptr};
    FilesModel *_filesModel;
    Q_PROPERTY(bool hideUnchangeds READ hideUnchangeds WRITE setHideUnchangeds NOTIFY hideUnchangedsChanged)

public:
    explicit DiffTreeView(QWidget *parent = nullptr);

    DiffTreeModel *diffModel() const;
    void setModels(DiffTreeModel *newDiffModel, FilesModel *filesModel);

    bool hideUnchangeds() const;
    void setHideUnchangeds(bool newHideUnchangeds);
    bool eventFilter(QObject *watched, QEvent *event) override;

private slots:
    void on_lineEditFilter_textChanged(QString text);
    void on_treeView_clicked(const QModelIndex &index);
    void on_listView_clicked(const QModelIndex &index);

signals:
    void fileSelected(const QString &file);
    void hideUnchangedsChanged();

};

#endif // DIFFTREEVIEW_H
