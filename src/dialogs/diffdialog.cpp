#include "diffdialog.h"

#include <git/gitmanager.h>
#include <QDebug>
#include <QFileDialog>
#include "models/difftreemodel.h"

DiffDialog::DiffDialog(QWidget *parent) :
      AppDialog(parent)
{
    setupUi(this);
}

DiffDialog::DiffDialog(const Git::File &oldFile, const Git::File &newFile, QWidget *parent) : AppDialog(parent),
      _oldFile(oldFile),
      _newFile(newFile)
{
    setupUi(this);
    diffWidget->setOldFile(oldFile);
    diffWidget->setNewFile(newFile);

    lineEditOldFileName->setText(oldFile.displayName());
    lineEditNewFileName->setText(newFile.displayName());

    diffWidget->compare();

    treeView->hide();
}

DiffDialog::DiffDialog(const QString &oldBranch, const QString &newBranch, QWidget *parent) : AppDialog(parent)
      , _oldBranch(oldBranch), _newBranch(newBranch)
{
    setupUi(this);

    _diffModel = new DiffTreeModel;
    auto diffs = Git::Manager::instance()->diffBranches(oldBranch, newBranch);

    for (auto &f: diffs)
        _diffModel->addFile(f);
    treeView->setModel(_diffModel);
}


void DiffDialog::on_toolButtonShowHiddenChars_clicked(bool checked)
{
    diffWidget->showHiddenChars(checked);
}


void DiffDialog::on_pushButtonSaveAs_clicked()
{
    auto diff = Git::Manager::instance()->diff(_oldFile.fileName(),_newFile.fileName());
    qDebug().noquote() << diff;
    auto fileName = QFileDialog::getSaveFileName(this, i18n("Save diff"));
    if (!fileName.isEmpty()) {
        QFile f(fileName);
        if (!f.open(QIODevice::WriteOnly)) {
            return;
        }
        f.write(diff.toUtf8());
        f.close();
    }
}


void DiffDialog::on_treeView_clicked(const QModelIndex &index)
{
    auto fileName = _diffModel->fullPath(index);

    Git::File oldFile(_oldBranch, fileName);
    Git::File newFile(_newBranch, fileName);
    diffWidget->setOldFile(oldFile);
    diffWidget->setNewFile(newFile);
    diffWidget->compare();

    lineEditOldFileName->setText(oldFile.displayName());
    lineEditNewFileName->setText(newFile.displayName());
}

