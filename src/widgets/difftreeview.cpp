#include "difftreeview.h"
#include "models/difftreemodel.h"
#include <QSortFilterProxyModel>
#include "models/filesmodel.h"
#include <QDebug>

DiffTreeModel *DiffTreeView::diffModel() const
{
    return _diffModel;
}

void DiffTreeView::setDiffModel(DiffTreeModel *newDiffModel, FilesModel *filesModel)
{
    _diffModel = newDiffModel;
    _filesModel = filesModel;
    _filterModel->setSourceModel(filesModel);
    treeView->setModel(_diffModel);
    listView->setModel(_filterModel);
}

DiffTreeView::DiffTreeView(QWidget *parent) :
      QWidget(parent)
{
    setupUi(this);
    _filterModel = new QSortFilterProxyModel(this);
    _filterModel->setFilterKeyColumn(0);
    connect(checkBoxHideUnchangeds, &QAbstractButton::toggled, this, &DiffTreeView::hideUnchangedsChanged);
    checkBoxHideUnchangeds->hide();
}

void DiffTreeView::on_lineEditFilter_textChanged(QString text)
{
    stackedWidget->setCurrentIndex(text.isEmpty() ? 0 : 1);
    _filterModel->setFilterWildcard("*" + text + "*");
}

void DiffTreeView::on_treeView_clicked(const QModelIndex &index)
{
    auto fileName = _diffModel->fullPath(index);
    emit fileSelected(fileName);
}

void DiffTreeView::on_listView_clicked(const QModelIndex &index)
{
    auto row = _filterModel->mapToSource(index).row();
    auto fileName = _filesModel->data(_filesModel->index(row, 1), Qt::DisplayRole);
    emit fileSelected(fileName.toString());
}

bool DiffTreeView::hideUnchangeds() const
{
    return checkBoxHideUnchangeds->isChecked();
}

void DiffTreeView::setHideUnchangeds(bool newHideUnchangeds)
{
    if (checkBoxHideUnchangeds->isChecked() == newHideUnchangeds)
        return;
    checkBoxHideUnchangeds->setChecked(newHideUnchangeds);
    emit hideUnchangedsChanged();
}
