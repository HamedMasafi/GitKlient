/*
Copyright (C) 2021 by Hamed Masafi <hamed.masafi@gmail.com>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of
the License or (at your option) version 3 or any later version
accepted by the membership of KDE e.V. (or its successor approved
by the membership of KDE e.V.), which shall act as a proxy 
defined in Section 14 of version 3 of the license.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

// application headers
#include "diffwindow.h"
#include "gitklientwindow.h"

#include "dialogs/changedfilesdialog.h"
#include "dialogs/clonedialog.h"
#include "dialogs/commitpushdialog.h"
#include "dialogs/filestreedialog.h"
#include "dialogs/mergedialog.h"
#include "dialogs/pulldialog.h"
#include "dialogs/runnerdialog.h"
#include "dialogs/selectbranchestodiffdialog.h"
#include "dialogs/searchdialog.h"
#include "git/gitmanager.h"
#include "gitklientdebug.h"
#include "gitklientview.h"
#include "mainwidget.h"
#include "multipagewidget.h"
#include "widgets/branchesstatuswidget.h"
#include "widgets/commitswidget.h"
#include "widgets/historyviewwidget.h"
#include "widgets/remoteswidget.h"
#include "widgets/stasheswidget.h"
#include "widgets/submoduleswidget.h"
#include "widgets/tagswidget.h"

// KF headers
#include <KActionCollection>
#include <KConfigDialog>
#include <KMessageBox>
#include <QFileDialog>
#include <QMenu>
#include <QSettings>
#include <QtConcurrent/QtConcurrent>



GitKlientWindow::GitKlientWindow()
    : KXmlGuiWindow()
{
    initActions();
    _mainWidget = new MultiPageWidget(this);
    _mainWidget->setDefaultGitManager(Git::Manager::instance());
    addPage<HistoryViewWidget>("view_overview");
    addPage<BranchesStatusWidget>("view_branches");
    addPage<CommitsWidget>("view_commits");
    addPage<StashesWidget>("view_stashes");
    addPage<SubmodulesWidget>("view_submodules");
    addPage<RemotesWidget>("view_remotes");
    addPage<TagsWidget>("view_tags");

    setupGUI(StandardWindowOption::Default, "gitklientui.rc");
    _mainWidget->setCurrentIndex(0);

    setCentralWidget(_mainWidget);

    if (GitKlientSettings::openLastRepo()) {
        QSettings s;
        auto p = s.value("last_repo").toString();
        Git::Manager::instance()->setPath(p);
        initRecentFiles(p);
        QtConcurrent::run(this, &GitKlientWindow::loadRemotes);
    }
}

GitKlientWindow::~GitKlientWindow()
{
}

void GitKlientWindow::settingsConfigure()
{
    if (KConfigDialog::showDialog(QStringLiteral("settings"))) {
        return;
    }

    KConfigDialog *dialog = new KConfigDialog(this, QStringLiteral("settings"), GitKlientSettings::self());

    QWidget *generalSettingsPage = new QWidget;
    settingsBase.setupUi(generalSettingsPage);
    dialog->addPage(generalSettingsPage, i18n("General"), QStringLiteral("package_setting"));

    auto diffSettingsPage = new QWidget;
    diffSettings.setupUi(diffSettingsPage);
    dialog->addPage(diffSettingsPage, i18n("Diff"), QStringLiteral("package_setting"));

    //    connect(dialog, &KConfigDialog::settingsChanged, m_kde_actionsView, &KlientView::handleSettingsChanged);
    dialog->show();
}

void GitKlientWindow::initActions()
{
    KActionCollection* actionCollection = this->actionCollection();

    auto branchBrowseAction = actionCollection->addAction(QStringLiteral("branch_browse"));
    branchBrowseAction->setText(i18n("Browse..."));
    connect(branchBrowseAction, &QAction::triggered, this, &GitKlientWindow::browseBranch);
    {
        auto repoStatusAction = actionCollection->addAction(QStringLiteral("repo_status"));
        repoStatusAction->setText(i18n("Status..."));
        connect(repoStatusAction, &QAction::triggered, this, &GitKlientWindow::repoStatus);
        repoStatusAction->setIcon(QIcon::fromTheme(QStringLiteral("folder-open")));
        actionCollection->setDefaultShortcut(repoStatusAction, QKeySequence("Ctrl+S"));
    }
    {
        recentAction = actionCollection->addAction("recent");
        recentAction->setText(i18n("Recent files"));
        recentAction->setMenu(new QMenu(this));
        initRecentFiles();
    }
    {
        auto commitOrPushAction =actionCollection->addAction("commit_or_push");
        commitOrPushAction->setText(i18n("Commit/Push"));
        connect(commitOrPushAction, &QAction::triggered, this, &GitKlientWindow::commitPushAction);
    }

    auto branchPullAction = actionCollection->addAction("branch_pull");
    branchPullAction->setText(i18n("Pull..."));
    connect(branchPullAction, &QAction::triggered, this, &GitKlientWindow::pull);

    auto branchDeleteAction = actionCollection->addAction("branch_delete");
    branchDeleteAction->setText(i18n("Delete"));
    connect(branchDeleteAction, &QAction::triggered, this, &GitKlientWindow::deleteBranch);

    auto branchCheckout = actionCollection->addAction(QStringLiteral("branch_checkout"));
    branchCheckout->setText(i18n("Checkout..."));

    auto file = actionCollection->addAction(QStringLiteral("file"));
    file->setText(i18n("Repo"));

    auto openRepoAction = actionCollection->addAction(QStringLiteral("open_repo"));
    openRepoAction->setText(i18n("Open repo"));
    actionCollection->setDefaultShortcut(openRepoAction, QKeySequence("Ctrl+O"));
    openRepoAction->setIcon(QIcon::fromTheme(QStringLiteral("folder-open")));
    connect(openRepoAction, &QAction::triggered, this, &GitKlientWindow::openRepo);

    auto branchStatusAction = actionCollection->addAction("branch_status");
    branchStatusAction->setText(i18n("Status..."));
    //connect(branchStatusAction, &QAction::triggered, this, &GitKlientWindow::showBranchesStatus);

    auto repoPullAction = actionCollection->addAction("repo_pull", this, &GitKlientWindow::pull);
    repoPullAction->setText(i18n("Pull..."));

    auto repoPushAction = actionCollection->addAction("repo_push", this, &GitKlientWindow::commitPushAction);
    repoPushAction->setText(i18n("Push..."));

    auto diffBranchesAction = actionCollection->addAction("diff_branches", this, &GitKlientWindow::diffBranches);
    diffBranchesAction->setText(i18n("Diff branches..."));

    auto repoSearchAction = actionCollection->addAction("repo_search", this, &GitKlientWindow::search);
    repoSearchAction->setText(i18n("Search..."));

    KStandardAction::quit(qApp, SLOT(closeAllWindows()), actionCollection);
    KStandardAction::preferences(this, SLOT(settingsConfigure()), actionCollection);
    KStandardAction::openNew(this, &GitKlientWindow::clone, actionCollection);

    auto branchesMenu = new QMenu(this);
    branchesMenu->addAction(branchBrowseAction);
    branchesMenu->addSeparator();
    branchesMenu->addAction(branchPullAction);
    branchesMenu->addAction(branchCheckout);
    branchesMenu->addSeparator();
    branchesMenu->addAction(branchDeleteAction);
    //    m_kde_actionsView->setBranchesContextMenu(branchesMenu);

}
void GitKlientWindow::initRecentFiles(const QString &newItem)
{
    recentAction->menu()->clear();
    QSettings s;
    auto recentList = s.value("recent_files").toStringList();
    if (newItem != QString()) {
        recentList.removeOne(newItem);
        recentList.prepend(newItem);
        s.setValue("recent_files", recentList);
        s.setValue("last_repo", newItem);
        s.sync();
    }
    for (auto &item: recentList) {
        auto action = recentAction->menu()->addAction(item);
        action->setData(item);
        connect(action, &QAction::triggered, this, &GitKlientWindow::recentActionTriggered);
    }
}

void GitKlientWindow::loadRemotes()
{
    auto git = Git::Manager::instance();
    auto remotes = git->remotes();
    for (auto &r: remotes)
        volatile auto remote = git->remoteDetails(r);
}


void GitKlientWindow::browseBranch()
{
    auto branch = "";//m_kde_actionsView->selectedBranch();
    if (branch == QString())
        return;

    FilesTreeDialog d(branch, this);
    d.exec();
}

void GitKlientWindow::repoStatus()
{
    ChangedFilesDialog d(this);
    d.exec();
}

void GitKlientWindow::openRepo()
{
    QFileDialog d;
    d.setFileMode(QFileDialog::Directory);
    if (d.exec()==QDialog::Accepted) {
        Git::Manager::instance()->setPath(d.directoryUrl().toLocalFile());
        //        m_kde_actionsView->reload();
        initRecentFiles(d.directoryUrl().toLocalFile());
    }
}

void GitKlientWindow::recentActionTriggered()
{
    auto action = qobject_cast<QAction*>(sender());
    if (!action)
        return;

    auto p = action->data().toString();
    Git::Manager::instance()->setPath(p);

    initRecentFiles(p);

    QtConcurrent::run(this, &GitKlientWindow::loadRemotes);
}

void GitKlientWindow::commitPushAction()
{
    CommitPushDialog d(this);
    d.exec();
}

void GitKlientWindow::deleteBranch()
{
    auto r = KMessageBox::warningYesNo(this,
                                       i18n("Are you sure to delete the selected branch?"));

    if (r == KMessageBox::No)
        return;

    //    qDebug() << "You choosed yes" << m_kde_actionsView->selectedBranch();
}

void GitKlientWindow::pull()
{
    qDebug() << Q_FUNC_INFO;
    PullDialog d(this);
    if (d.exec() == QDialog::Accepted) {
        RunnerDialog r(this);
        auto branch = Git::Manager::instance()->currentBranch();
        r.run({"pull", "origin", branch});
        r.exec();
    }
}

void GitKlientWindow::showBranchesStatus()
{
    MergeDialog d(this);
    d.exec();
}

void GitKlientWindow::clone()
{
    CloneDialog d(this);
    d.exec();
}

void GitKlientWindow::diffBranches()
{
    SelectBranchesToDiffDialog d(Git::Manager::instance(), this);
    if (d.exec() == QDialog::Accepted) {
        auto diffWin = new DiffWindow(d.oldBranch(), d.newBranch());
        diffWin->setWindowModality(Qt::ApplicationModal);
        diffWin->setAttribute(Qt::WA_DeleteOnClose, true);
        diffWin->show();
    }
}

void GitKlientWindow::search()
{
    SearchDialog d(Git::Manager::instance(), this);
    d.exec();
}

template<class T>
void GitKlientWindow::addPage(const QString &actionName)
{
    const QList<Qt::Key> keys = {
        Qt::Key_0,
        Qt::Key_1,
        Qt::Key_2,
        Qt::Key_3,
        Qt::Key_4,
        Qt::Key_5,
        Qt::Key_6,
        Qt::Key_7,
        Qt::Key_8,
        Qt::Key_9
    };
    auto action = actionCollection()->addAction(actionName);
    auto w = new T(this);
    action->setText(w->windowTitle());
    action->setIcon(QIcon::fromTheme(actionName));
    if (_mainWidget->count() < 10)
        actionCollection()->setDefaultShortcut(action, QKeySequence(Qt::CTRL + keys[_mainWidget->count()]));

    _mainWidget->addPage(w, action);
}
