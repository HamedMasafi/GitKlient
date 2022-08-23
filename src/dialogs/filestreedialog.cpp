#include "filestreedialog.h"
#include "models/treemodel.h"
#include "git/gitmanager.h"
#include <QDebug>
#include <QFileIconProvider>
#include <QMenu>
#include "dialogs/fileviewerdialog.h"
#include "dialogs/filehistorydialog.h"
#include "dialogs/searchdialog.h"
#include "actions/fileactions.h"

FilesTreeDialog::FilesTreeDialog(const QString &place, QWidget *parent) :
      AppDialog(parent), _place(place)
{
    setupUi(this);

    _actions = new FileActions(Git::Manager::instance(), this);
    _actions->setPlace(place);

    _treeModel = new TreeModel(this);
    _treeModel->setSeparator("/");

    auto files = Git::Manager::instance()->ls(place);

    _treeModel->setLastPartAsData(true);
    QFileIconProvider p;
    _treeModel->setDefaultIcon(p.icon(QFileIconProvider::Folder));
    _treeModel->addData(files);
    treeView->setModel(_treeModel);

    setWindowTitle(QStringLiteral("Browse files: %1").arg(place));

    lineEditBranchName->setText(place);

    listWidget->clear();

    for (auto &f: _treeModel->rootData()) {
        QFileInfo fi(f);
        auto icon = p.icon(fi);
        auto item = new QListWidgetItem(listWidget);
        item->setText(f);
        item->setIcon(icon);
        listWidget->addItem(item);
    }
}

void FilesTreeDialog::on_treeView_clicked(const QModelIndex &index)
{
    QFileIconProvider p;
    listWidget->clear();

    for (auto &f: _treeModel->data(index)) {
        QFileInfo fi(f);
        auto icon = p.icon(fi);
        auto item = new QListWidgetItem(listWidget);
        item->setText(f);
        item->setIcon(icon);
        listWidget->addItem(item);
    }
}

void FilesTreeDialog::on_listWidget_customContextMenuRequested(const QPoint &pos)
{
    auto path = _treeModel->fullPath(treeView->currentIndex());

    if (path=="/")
        path = listWidget->currentItem()->text();
    else
        path += "/" + listWidget->currentItem()->text();

    _actions->setFilePath(path);
    _actions->popup(listWidget->mapToGlobal(pos));
}
