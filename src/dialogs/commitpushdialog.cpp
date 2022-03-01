#include "commitpushdialog.h"
#include "runnerdialog.h"
#include "git/gitmanager.h"
#include "git/gitfile.h"
#include "git/commandcommit.h"
#include "git/commandpush.h"
#include "dialogs/diffdialog.h"
#include <KTextEditor/Document>
#include <KTextEditor/Editor>
#include <KTextEditor/View>
#include <KTextEditor/CodeCompletionModel>
#include <KService>

CommitPushDialog::CommitPushDialog(QWidget *parent) :
      QDialog(parent)
{
    setupUi(this);

    auto git = Git::Manager::instance();

    auto files = git->changedFiles();

    for (auto i = files.begin(); i != files.end(); ++i) {
        auto item = new QListWidgetItem(listWidget);
        item->setText(i.key());
        switch (i.value()) {
        case Git::Manager::Modified:
            item->setForeground(Qt::blue);
            break;
        case Git::Manager::Added:
            item->setForeground(Qt::green);
            break;
        case Git::Manager::Removed:
            item->setForeground(Qt::red);
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

//    KTextEditor::Editor *editor = KTextEditor::Editor::instance();
//    // create a new document
//    KTextEditor::Document *doc = editor->createDocument(this);
//    // create a widget to display the document
//    KTextEditor::View *view = doc->createView(widget_2);

//    auto completionModel = new KTextEditor::CodeCompletionModel(this);
    //    KService::Ptr service = KService::serviceByDesktopName("katepart");
    //    if (service) {
    //        auto m_part = service->createInstance<KParts::ReadWritePart>();
    //        m_part->embed(widget_2);
    //    }
//    textEditMessage->hide();
//    listWidget->hide();
}

void CommitPushDialog::checkButtonsEnable()
{
    auto enable = checkBoxAmend->isChecked() || !textEditMessage->toPlainText().isEmpty();
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

//    Git::Manager::instance()->commit(textEditMessage->toPlainText());
    Git::Manager::instance()->run(cmd);
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

    Git::Manager::instance()->commit(textEditMessage->toPlainText());
    RunnerDialog d(this);
    d.run(&cmd);
    d.exec();
    close();
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
    auto git = Git::Manager::instance();
    for (auto i = 0; i < listWidget->count(); ++i) {
        auto item = listWidget->item(i);
        if (item->checkState() == Qt::Checked)
            git->addFile(item->text());
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
    Git::File original{Git::Manager::instance()->currentBranch(), item->text()};
    Git::File changed{Git::Manager::instance()->path() + "/" + item->text()};
    DiffDialog d(original, changed, this);
    d.exec();
}


