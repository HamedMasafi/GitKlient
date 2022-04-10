#include "remotesactions.h"

#include <klocalizedstring.h>
#include <kmessagebox.h>

#include "git/models/remotescache.h"
#include "git/gitmanager.h"
#include "widgets/remoteinfodialog.h"

RemotesActions::RemotesActions(Git::Manager *git, QWidget *parent) : AbstractActions(git, parent)
{
    _actionCreate = addActionHidden(i18n("New..."), this, &RemotesActions::create);
    _actionRemove = addActionDisabled(i18n("Remove..."), this, &RemotesActions::remove);
}

const QString &RemotesActions::remoteName() const
{
    return _remoteName;
}

void RemotesActions::setRemoteName(const QString &newRemoteName)
{
    _remoteName = newRemoteName;

    setActionEnabled(_actionRemove, true);
}

void RemotesActions::create()
{
    RemoteInfoDialog d{_parent};
    if (d.exec() == QDialog::Accepted) {
        _git->addRemote(d.remoteName(), d.remoteUrl());
        _git->remotesModel()->load();
    }
}

void RemotesActions::remove()
{
    auto r = KMessageBox::questionYesNo(_parent,
                                        i18n("Are you sure to remove the selected remote?"),
                                        i18n("Remove remote?"));

    if (r == KMessageBox::Yes) {
        _git->removeRemote(_remoteName);
        _git->remotesModel()->load();
    }

}
