#include "fileviewerdialog.h"
#include "searchdialog.h"

#include <git/gitlog.h>
#include <git/gitmanager.h>
#include <QStandardItemModel>
#include <QtConcurrent>
#include <KLocalizedString>

SearchDialog::SearchDialog(const QString &path, Git::Manager *git, QWidget *parent)
    : AppDialog(parent), _git(git)
{
    setupUi(this);
    initModel();
    lineEditPath->setText(path);
}

void SearchDialog::initModel()
{
    if (!_model) {
        _model = new QStandardItemModel(this);
        treeView->setModel(_model);
    }

    _model->setColumnCount(3);
    _model->setHeaderData(0, Qt::Horizontal, i18n("File name"));
    _model->setHeaderData(1, Qt::Horizontal, i18n("Branch"));
    _model->setHeaderData(2, Qt::Horizontal, i18n("Commit"));

}

SearchDialog::SearchDialog(Git::Manager *git, QWidget *parent) : AppDialog(parent), _git(git)
{
    setupUi(this);
    initModel();
}

void SearchDialog::on_pushButtonSearch_clicked()
{
    _model->clear();
    initModel();
    startTimer(500);
    pushButtonSearch->setEnabled(false);
    _progress.total = _progress.value = 0;
    auto f = QtConcurrent::run(this, &SearchDialog::beginSearch);
}

void SearchDialog::on_treeView_doubleClicked(QModelIndex index)
{
    if (!index.isValid())
        return;
    auto file = _model->data(_model->index(index.row(), 0)).toString();
    auto branch = _model->data(_model->index(index.row(), 1)).toString();
    auto commit = _model->data(_model->index(index.row(), 2)).toString();

    QString place;
    if (!commit.isEmpty() && !branch.isEmpty())
        place = branch + ":" + commit;
    else
        place = branch.isEmpty() ? commit : branch;

    auto d = new FileViewerDialog(place, file);
    d->setWindowModality(Qt::ApplicationModal);
    d->setAttribute(Qt::WA_DeleteOnClose, true);
    d->show();
}

void SearchDialog::beginSearch()
{
    if (radioButtonSearchBranches->isChecked()) {
        auto branchesList = _git->branches();
        _progress.total = branchesList.size();
        for (auto &branch: branchesList) {
            searchOnPlace(branch, QString());
            _progress.value++;
        }
    } else {
        Git::LogList list;
        list.load();

        _progress.total = list.size();
        for (auto &branch: list) {
            searchOnPlace(QString(), branch->commitHash());
            _progress.value++;
        }
    }

    pushButtonSearch->setEnabled(true);
}

void SearchDialog::searchOnPlace(const QString &branch, const QString &commit)
{
    QString place = branch.isEmpty() ? commit : branch;
    auto files = _git->ls(place);

    for (auto &file: files) {
        if (!lineEditPath->text().isEmpty() && !file.contains(lineEditPath->text()))
            continue;

        bool ok = _git->fileContent(place, file)
                      .contains(lineEditText->text(),
                                checkBoxCaseSensetive->isChecked() ? Qt::CaseSensitive
                                                                   : Qt::CaseInsensitive);
        if (ok) {
            _model->appendRow(
                {new QStandardItem(file), new QStandardItem(branch), new QStandardItem(commit)});
        }
    }
}

void SearchDialog::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event)
    progressBar->setMaximum(_progress.total);
    progressBar->setValue(_progress.value);
}
