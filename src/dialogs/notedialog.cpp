#include "notedialog.h"

#include "git/gitmanager.h"

NoteDialog::NoteDialog(Git::Manager *git, const QString &branchName, QWidget *parent) :
      AppDialog(git, parent), _branchName{branchName}
{
    setupUi(this);

    textEdit->setText(git->readNote(branchName));
}

void NoteDialog::on_buttonBox_accepted()
{
    _git->saveNote(_branchName, textEdit->toPlainText());
    close();
}
