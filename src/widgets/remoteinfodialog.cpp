#include "remoteinfodialog.h"
#include "git/commands/commandaddremote.h"

RemoteInfoDialog::RemoteInfoDialog(QWidget *parent) :
      QDialog(parent)
{
    setupUi(this);

    checkBoxTags->setCheckState(Qt::PartiallyChecked);
}

QString RemoteInfoDialog::remoteName() const
{
    return lineEditName->text();
}

QString RemoteInfoDialog::remoteUrl() const
{
    return lineEditUrl->text();
}

Git::CommandAddRemote *RemoteInfoDialog::command()
{
    auto cmd = new Git::CommandAddRemote(this);
    cmd->setTags(checkBoxTags->checkState());
    cmd->setRemoteName(lineEditName->text());
    cmd->setUrl(lineEditUrl->text());
    cmd->setMirror(checkBoxMirror->isChecked());
    cmd->setFetch(checkBoxFetch->isChecked());
    return cmd;
}
