#include "fileblamedialog.h"

#include "git/gitmanager.h"

FileBlameDialog::FileBlameDialog(Git::Manager *git, const QString &fileName, QWidget *parent)
    : Dialog(parent), _git(git), _fileName(fileName)
{
    setupUi(this);

    plainTextEdit->setHighlighting(fileName);
    auto content = _git->fileContent("", _fileName);
    plainTextEdit->setPlainText(content);

    setWindowTitle(i18n("Blame file: %1").arg(fileName));
}

FileBlameDialog::FileBlameDialog(const Git::File &file, QWidget *parent)
    : Dialog(parent), _file(file)
{
    setupUi(this);
    plainTextEdit->setHighlighting(file.fileName());
//    plainTextEdit->setPlainText(file.content());

    auto b = file.git()->blame(file);
    plainTextEdit->setBlameData(b);

    b.initCommits(file.git()->logs());
    setWindowTitle(i18n("Blame file: %1", file.fileName()));
}
