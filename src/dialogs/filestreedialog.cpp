#include "filestreedialog.h"
#include "models/treemodel.h"
#include "git/gitmanager.h"
#include <QDebug>
#include <QFileIconProvider>
#include <QMenu>
#include <KIOCore/KFileItem>
#include "dialogs/fileviewerdialog.h"

FilesTreeDialog::FilesTreeDialog(const QString &place, QWidget *parent) :
      QDialog(parent), _place(place)
{
    setupUi(this);

    _treeModel = new TreeModel(this);
    _treeModel->setSeprator("/");

    auto files = Git::Manager::instance()->ls(place);

    _treeModel->setLastPartAsData(true);
    QFileIconProvider p;
    _treeModel->setDefaultIcon(p.icon(QFileIconProvider::Folder));
    _treeModel->addData(files.split("\n"));
    treeView->setModel(_treeModel);

    setWindowTitle(QStringLiteral("Browse files: %1").arg(place));

    lineEditBranchName->setText(place);

    _fileMenu = new QMenu(this);

    auto actionCopy = new QAction(this);
    actionCopy->setText(i18n("Copy"));
    connect(actionCopy, &QAction::triggered, this, &FilesTreeDialog::copyFile);
    _fileMenu->addAction(actionCopy);

    auto actionView = new QAction(this);
    actionView->setText(i18n("View"));
    connect(actionView, &QAction::triggered, this, &FilesTreeDialog::viewFile);
    _fileMenu->addAction(actionView);
}

void FilesTreeDialog::on_treeView_clicked(const QModelIndex &index)
{
    QFileIconProvider p;
    listWidget->clear();

//    listWidget->addItems(_treeModel->data(index));
    for (auto &f: _treeModel->data(index)) {
        QFileInfo fi(f);
        auto icon = p.icon(fi);
        auto item = new QListWidgetItem(listWidget);
        item->setText(f);
        item->setIcon(icon);
        listWidget->addItem(item);
    }
}

void FilesTreeDialog::viewFile()
{
    auto path = _treeModel->fullPath(treeView->currentIndex()) + "/"
                + listWidget->currentItem()->text();
    qDebug() << "p=" << path;
    FileViewerDialog d(_place, path);
    d.exec();
}

void FilesTreeDialog::copyFile()
{
    auto path = _treeModel->fullPath(treeView->currentIndex()) + "/"
                + listWidget->currentItem()->text();

}


void FilesTreeDialog::on_listWidget_customContextMenuRequested(const QPoint &pos)
{
    _fileMenu->popup(listWidget->mapToGlobal(pos));
}

