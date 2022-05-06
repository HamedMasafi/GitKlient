#include "commitpushdialog.h"
#include "dialogs/diffdialog.h"
#include "git/commands/commandcommit.h"
#include "git/commands/commandpush.h"
#include "git/gitfile.h"
#include "git/gitmanager.h"
#include "runnerdialog.h"
#include "actions/changedfileactions.h"
#include "GitKlientSettings.h"

#include <KService>
#include <KTextEditor/CodeCompletionModel>
#include <KTextEditor/Document>
#include <KTextEditor/Editor>
#include <KTextEditor/View>

CommitPushDialog::CommitPushDialog(Git::Manager *git, QWidget *parent) :
      Dialog(parent), _git(git)
{
    setupUi(this);

    auto files = git->changedFiles();

    for (auto i = files.begin(); i != files.end(); ++i) {
        auto item = new QListWidgetItem(listWidget);
        item->setText(i.key());
        switch (i.value()) {
        case Git::Manager::Modified:
            item->setForeground(GitKlientSettings::diffModifiedColor());
            break;
        case Git::Manager::Added:
            item->setForeground(GitKlientSettings::diffAddedColor());
            break;
        case Git::Manager::Removed:
            item->setForeground(GitKlientSettings::diffRemovedColor());
            break;

        default:
            break;
        }
        item->setCheckState(Qt::Unchecked);
        item->setData(1, i.value());
        listWidget->addItem(item);
    }
//    textEditMessage->setSpellCheckingLanguage("en_US");
//    textEditMessage->setCheckSpellingEnabled(true);

    auto branches = git->branches();
    auto remotes = git->remotes();
    comboBoxBranch->addItems(branches);
    comboBoxRemote->addItems(remotes);
    labelCurrentBranchName->setText(git->currentBranch());

    QSet<QString> _words;
    for (auto &b: branches)
        _words.insert(b);
    for (auto &r: remotes)
        _words.insert(r);
    for (auto i = files.begin(); i != files.end(); ++i) {
        auto parts = i.key().split("/");
        for (auto &p: parts)
            _words.insert(p);
        _words.insert(i.key());
    }
    textEditMessage->addWords(_words.values());
    textEditMessage->begin();

    _actions = new ChangedFileActions(_git, this);
}

void CommitPushDialog::checkButtonsEnable()
{
    if (!groupBoxMakeCommit->isChecked()) {
        pushButtonCommit->setEnabled(false);
        pushButtonPush->setEnabled(true);
        return;
    }
    bool enable{false};

    for (auto i = 0; i < listWidget->count(); ++i) {
        if (listWidget->item(i)->checkState() == Qt::Checked) {
            enable = true;
            break;
        }
    }

    if (!enable) {
        pushButtonCommit->setEnabled(false);
        pushButtonPush->setEnabled(false);
        return;
    }

    enable = checkBoxAmend->isChecked() || !textEditMessage->toPlainText().isEmpty();
    pushButtonCommit->setEnabled(enable);
    pushButtonPush->setEnabled(enable);
}

void CommitPushDialog::on_pushButtonCommit_clicked()
{
    addFiles();
    Git::CommandCommit cmd;
    cmd.setAmend(checkBoxAmend->isChecked());
    cmd.setMessage(textEditMessage->toPlainText());
    cmd.setIncludeStatus(checkBoxIncludeStatus->isChecked());

//    _git->commit(textEditMessage->toPlainText());
    _git->run(cmd);
    accept();
}

void CommitPushDialog::on_pushButtonPush_clicked()
{
    addFiles();

    Git::CommandPush cmd;
    cmd.setRemote(comboBoxRemote->currentText());

    if (radioButtonCurrentBranch->isChecked())
        cmd.setLocalBranch(labelCurrentBranchName->text());
    else if (radioButtonExistingBranch->isChecked())
        cmd.setLocalBranch(comboBoxBranch->currentText());
    else
        cmd.setLocalBranch(lineEditNewBranchName->text());
    cmd.setForce(checkBoxForce->isChecked());

    _git->commit(textEditMessage->toPlainText());
    RunnerDialog d(this);
    d.run(&cmd);
    d.exec();
    accept();
}

void CommitPushDialog::on_toolButtonAddAll_clicked()
{
    for (auto i = 0; i < listWidget->count(); ++i) {
        auto item = listWidget->item(i);
        item->setCheckState(Qt::Checked);
    }
}

void CommitPushDialog::addFiles()
{
    for (auto i = 0; i < listWidget->count(); ++i) {
        auto item = listWidget->item(i);
        if (item->checkState() == Qt::Checked)
            _git->addFile(item->text());
    }
}

void CommitPushDialog::on_toolButtonAddNone_clicked()
{
    for (auto i = 0; i < listWidget->count(); ++i) {
        auto item = listWidget->item(i);
        item->setCheckState(Qt::Unchecked);
    }
}


void CommitPushDialog::on_toolButtonAddIndexed_clicked()
{
    for (auto i = 0; i < listWidget->count(); ++i) {
        auto item = listWidget->item(i);
        item->setCheckState(Qt::Checked);
    }
}

void CommitPushDialog::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    if (!item)
        return;
    _actions->setFilePath(listWidget->currentItem()->text());
    _actions->diff();
}
void CommitPushDialog::on_listWidget_itemClicked(QListWidgetItem *)
{
    checkButtonsEnable();
}

void CommitPushDialog::on_groupBoxMakeCommit_toggled(bool)
{
    checkButtonsEnable();
}

void CommitPushDialog::on_listWidget_customContextMenuRequested(const QPoint &pos)
{
    Q_UNUSED(pos)
    if (listWidget->currentRow() == -1)
        return;

    _actions->setFilePath(listWidget->currentItem()->text());
    _actions->popup();
}
