#include "remotesactions.h"

#include <QInputDialog>
#include <klocalizedstring.h>
#include <kmessagebox.h>
#include <QDebug>

#include "dialogs/runnerdialog.h"
#include "git/commands/commandaddremote.h"
#include "git/gitmanager.h"
#include "git/models/remotesmodel.h"
#include "widgets/remoteinfodialog.h"

RemotesActions::RemotesActions(Git::Manager *git, QWidget *parent) : AbstractActions(git, parent)
{
    _actionCreate = addActionHidden(i18n("New..."), this, &RemotesActions::create);
    _actionRemove = addActionDisabled(i18n("Remove..."), this, &RemotesActions::remove);
    _actionRename = addActionDisabled(i18n("Rename..."), this, &RemotesActions::rename);
    _actionChangeUrl = addActionDisabled(i18n("Change url..."), this, &RemotesActions::changeUrl);
    _actionUpdate = nullptr;
}

const QString &RemotesActions::remoteName() const
{
    return _remoteName;
}

void RemotesActions::setRemoteName(const QString &newRemoteName)
{
    _remoteName = newRemoteName;

    setActionEnabled(_actionRemove, true);
    setActionEnabled(_actionRename, true);
    setActionEnabled(_actionChangeUrl, true);
}

void RemotesActions::create()
{
    RemoteInfoDialog d{_parent};
    if (d.exec() == QDialog::Accepted) {
//        _git->addRemote(d.remoteName(), d.remoteUrl());

        RunnerDialog runner;
        runner.run(d.command());
        runner.exec();
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

void RemotesActions::changeUrl()
{
    auto remote = _git->remotesModel()->findByName(_remoteName);

    if (!remote)
        return;
    auto newUrl = QInputDialog::getText(_parent,
                                        i18n("Change url"),
                                        "URL",
                                        QLineEdit::Normal,
                                        remote->pushUrl);

    if (!newUrl.isEmpty()) {
        _git->remotesModel()->setUrl(_remoteName, newUrl);
        KMessageBox::information(_parent, i18n("Url for remote changed successfully"));
    }
}

void RemotesActions::rename()
{
    auto newName = QInputDialog::getText(_parent,
                                         i18n("Rename"),
                                         "New name",
                                         QLineEdit::Normal,
                                         _remoteName);

    if (newName != QString()) {
        _git->remotesModel()->rename(_remoteName, newName);
        _remoteName = newName;
    }
}

void RemotesActions::update()
{

}
