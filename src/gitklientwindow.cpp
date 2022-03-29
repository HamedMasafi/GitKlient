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
#include "git/commands/commandpull.h"
#include "gitklientwindow.h"

#include "dialogs/changedfilesdialog.h"
#include "dialogs/clonedialog.h"
#include "dialogs/commitpushdialog.h"
#include "dialogs/fetchdialog.h"
#include "dialogs/filestreedialog.h"
#include "dialogs/mergedialog.h"
#include "dialogs/pulldialog.h"
#include "dialogs/reposettingsdialog.h"
#include "dialogs/runnerdialog.h"
#include "dialogs/searchdialog.h"
#include "dialogs/selectbranchestodiffdialog.h"
#include "git/gitmanager.h"
#include "git/models/logscache.h"
#include "gitklientdebug.h"
#include "gitklientview.h"
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
#include <KLocalizedString>


GitKlientWindow::GitKlientWindow()
    : MainWindow()
{
    _git = Git::Manager::instance();
    connect(_git, &Git::Manager::pathChanged, this, &GitKlientWindow::git_pathChanged);

    initActions();
    _mainWidget = new MultiPageWidget(this);
    _mainWidget->setDefaultGitManager(_git);
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
        _git->setPath(p);
        initRecentFiles(p);
        QtConcurrent::run(this, &GitKlientWindow::loadRemotes);
    }
}

GitKlientWindow::~GitKlientWindow()
{
    QSettings s;
    for (auto &w: _baseWidgets)
        w->saveState(s);
}

GitKlientWindow *GitKlientWindow::instance()
{
    static GitKlientWindow *instance = new GitKlientWindow;
    return instance;
}

void GitKlientWindow::git_pathChanged()
{
    setWindowFilePath(_git->path());
//    setWindowTitle(_git->path());
}

void GitKlientWindow::settingsConfigure()
{
    if (KConfigDialog::showDialog(QStringLiteral("settings"))) {
        return;
    }

    KConfigDialog *dialog = new KConfigDialog(this, QStringLiteral("settings"), GitKlientSettings::self());

    QWidget *generalSettingsPage = new QWidget;
    settingsBase.setupUi(generalSettingsPage);

#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    settingsBase.kcfg_calendarType->addItems(QCalendar::availableCalendars());
#else
    settingsBase.labelDefaultCalendar->hide();
    settingsBase.kcfg_calendarType->hide();
#endif

    //#ifndef QT_BOOTSTRAPPED
    //    settingsBase.kcfg_calendarType->addItem("Julian");
    //    settingsBase.kcfg_calendarType->addItem("Milankovic");
    //#endif
    //#if QT_CONFIG(jalalicalendar)
    //    settingsBase.kcfg_calendarType->addItem("Jalali");
    //#endif
    //#if QT_CONFIG(islamiccivilcalendar)
    //    settingsBase.kcfg_calendarType->addItem("IslamicCivil");
    //#endif
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

    auto repoStatusAction = actionCollection->addAction(QStringLiteral("repo_status"),
                                                        this,
                                                        &GitKlientWindow::repoStatus);
    repoStatusAction->setText(i18n("Changed files..."));
    actionCollection->setDefaultShortcut(repoStatusAction, QKeySequence("Ctrl+S"));

    {
        recentAction = actionCollection->addAction("recent");
        recentAction->setText(i18n("Recent files"));
        recentAction->setMenu(new QMenu(this));
        initRecentFiles();
    }

    auto openRepoAction = actionCollection->addAction(QStringLiteral("open_repo"),
                                                      this,
                                                      &GitKlientWindow::openRepo);
    openRepoAction->setText(i18n("Open repo"));
    actionCollection->setDefaultShortcut(openRepoAction, QKeySequence("Ctrl+O"));
    openRepoAction->setIcon(QIcon::fromTheme(QStringLiteral("folder-open")));

    auto repoPullAction = actionCollection->addAction("repo_pull", this, &GitKlientWindow::pull);
    repoPullAction->setText(i18n("Pull..."));

    auto repoFetchAction = actionCollection->addAction("repo_fetch", this, &GitKlientWindow::fetch);
    repoFetchAction->setText(i18n("Fetch..."));

    auto repoPushAction = actionCollection->addAction("repo_push", this, &GitKlientWindow::commitPushAction);
    repoPushAction->setText(i18n("Push..."));

    auto diffBranchesAction = actionCollection->addAction("diff_branches", this, &GitKlientWindow::diffBranches);
    diffBranchesAction->setText(i18n("Diff branches..."));

    auto repoSearchAction = actionCollection->addAction("repo_search", this, &GitKlientWindow::search);
    repoSearchAction->setText(i18n("Search..."));

    auto repoSettingsAction = actionCollection->addAction("repo_settings", this, &GitKlientWindow::repoSettings);
    repoSettingsAction->setText(i18n("Repo settings..."));

    KStandardAction::quit(this, &QMainWindow::close, actionCollection);
    KStandardAction::preferences(this, &GitKlientWindow::settingsConfigure, actionCollection);
    KStandardAction::openNew(this, &GitKlientWindow::clone, actionCollection);
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
    auto remotes = _git->remotes();
    for (auto &r: remotes)
        volatile auto remote = _git->remoteDetails(r);
}

void GitKlientWindow::repoStatus()
{
    ChangedFilesDialog d(_git, this);
    d.exec();
}

void GitKlientWindow::openRepo()
{
    QFileDialog d;
    d.setFileMode(QFileDialog::Directory);
    if (d.exec()==QDialog::Accepted) {
        _git->setPath(d.directoryUrl().toLocalFile());
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
    _git->setPath(p);

    initRecentFiles(p);

    QtConcurrent::run(this, &GitKlientWindow::loadRemotes);
}

void GitKlientWindow::commitPushAction()
{
    CommitPushDialog d(_git, this);
    if (d.exec() == QDialog::Accepted)
        _git->logsCache()->load();
}

void GitKlientWindow::pull()
{
    PullDialog d(this);
    d.exec();
}

void GitKlientWindow::fetch()
{
    FetchDialog d(_git, this);
    d.exec();
}

void GitKlientWindow::showBranchesStatus()
{
    MergeDialog d(this);
    d.exec();
}

void GitKlientWindow::clone()
{
    CloneDialog d(this);
    if (d.exec() == QDialog::Accepted) {
        RunnerDialog r(this);
        auto branch = _git->currentBranch();
        auto cmd = d.command();;
        r.run(cmd);
        r.exec();
        cmd->deleteLater();
    }
}

void GitKlientWindow::diffBranches()
{
    SelectBranchesToDiffDialog d(_git, this);
    if (d.exec() == QDialog::Accepted) {
        auto diffWin = new DiffWindow(d.oldBranch(), d.newBranch());
        diffWin->showModal();
    }
}

void GitKlientWindow::search()
{
    SearchDialog d(_git, this);
    d.exec();
}

void GitKlientWindow::repoSettings()
{
    RepoSettingsDialog d(_git, this);
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
    auto w = new T(_git, this);
    action->setText(w->windowTitle());
    action->setIcon(QIcon::fromTheme(actionName));
    if (_mainWidget->count() < 10)
        actionCollection()->setDefaultShortcut(action, QKeySequence(Qt::CTRL + keys[_mainWidget->count()]));

    _mainWidget->addPage(w, action);
    QSettings s;
    w->restoreState(s);
    _baseWidgets.append(w);
}
