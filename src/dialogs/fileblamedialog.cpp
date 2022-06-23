#include "fileblamedialog.h"

#include "git/gitmanager.h"
#include "git/models/logsmodel.h"
#include "qtextobject.h"

#include <klocalizedstring.h>

FileBlameDialog::FileBlameDialog(Git::Manager *git, const QString &fileName, QWidget *parent)
    : AppDialog(parent), _git(git), _fileName(fileName)
{
    setupUi(this);

    plainTextEdit->setHighlighting(fileName);
    auto content = _git->fileContent("", _fileName);
    plainTextEdit->setPlainText(content);

    setWindowTitle(i18n("Blame file: %1").arg(fileName));
    logDetailsWidget->setEnableCommitsLinks(false);
}

FileBlameDialog::FileBlameDialog(const Git::File &file, QWidget *parent)
    : AppDialog(parent), _git(Git::Manager::instance()), _file(file)
{
    setupUi(this);
    plainTextEdit->setHighlighting(file.fileName());
//    plainTextEdit->setPlainText(file.content());

    auto b = file.git()->blame(file);
    plainTextEdit->setBlameData(b);

//    b.initCommits(file.git()->logs());
    setWindowTitle(i18n("Blame file: %1", file.fileName()));

    logDetailsWidget->setEnableCommitsLinks(false);
}

void FileBlameDialog::on_plainTextEdit_blockSelected()
{
    auto b = plainTextEdit->blameData(plainTextEdit->textCursor().block().blockNumber());
    logDetailsWidget->setLog(_git->logsModel()->findLogByHash(b.commitHash));
}
