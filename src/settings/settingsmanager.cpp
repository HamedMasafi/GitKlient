#include "settingsmanager.h"

#include "GitKlientSettings.h"
#include "appwindow.h"
#include "git/gitmanager.h"

#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
#include <QCalendar>
#endif

#include <KConfigDialog>

#include <QAction>
#include <QWidget>

SettingsManager::SettingsManager() = default;

SettingsManager *SettingsManager::instance()
{
    static auto *instance = new SettingsManager;
    return instance;
}

void SettingsManager::settingsChanged()
{
    qDebug() << GitKlientSettings::calendarType() << pageBase.kcfg_calendarType->currentText();
    GitKlientSettings::setCalendarType(pageBase.kcfg_calendarType->currentText());

    auto git = Git::Manager::instance();

    if (GitKlientSettings::registerDiffTool()) {
        git->setConfig("difftool.gitklientdiff.cmd",
                       "gitklientdiff \"$LOCAL\" \"$REMOTE\"",
                       Git::Manager::ConfigGlobal);
    } else {
        git->unsetConfig("difftool.gitklientdiff.cmd", Git::Manager::ConfigGlobal);
    }
    if (GitKlientSettings::registerMergeTool()) {
        git->setConfig("mergetool.gitklientmerge.cmd",
                       "gitklientmerge \"$BASE\" \"$LOCAL\" \"$REMOTE\" \"$MERGED\"",
                       Git::Manager::ConfigGlobal);
        git->setConfig("mergetool.gitklientmerge.trustExitCode", "true", Git::Manager::ConfigGlobal);
    } else {
        git->unsetConfig("mergetool.gitklientmerge.cmd", Git::Manager::ConfigGlobal);
        git->unsetConfig("mergetool.gitklientmerge.trustExitCode", Git::Manager::ConfigGlobal);
    }
}

void SettingsManager::show()
{
    exec(AppWindow::instance());
}

QWidget *SettingsManager::createBasePage()
{
    auto w = new QWidget;
    pageBase.setupUi(w);

#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    pageBase.kcfg_calendarType->addItems(QCalendar::availableCalendars());
    pageBase.kcfg_calendarType->setCurrentText(GitKlientSettings::calendarType());
#else
    pageBase.labelDefaultCalendar->hide();
    pageBase.kcfg_calendarType->hide();
#endif
    return w;
}
QWidget *SettingsManager::createDiffPage()
{
    auto w = new QWidget;
    pageDiff.setupUi(w);
    return w;
}
QWidget *SettingsManager::createGitPage()
{
    auto w = new QWidget;
    pageGit.setupUi(w);
    return w;
}

void SettingsManager::exec(QWidget *parentWidget)
{
    auto name = QStringLiteral("settings");
    auto dialog = KConfigDialog::exists(name);

    if (dialog) {
        dialog->setParent(parentWidget);
        dialog->exec();
        return;
    }
//    if (KConfigDialog::showDialog(QStringLiteral("settings"))) {
//        return;
//    }

    dialog = new KConfigDialog(parentWidget, name, GitKlientSettings::self());

    dialog->addPage(createBasePage(), i18n("General"), QStringLiteral("package_setting"));
    dialog->addPage(createDiffPage(), i18n("Diff"), QStringLiteral("package_setting"));
    dialog->addPage(createGitPage(), i18n("Git"), QStringLiteral("package_setting"));

    connect(dialog, &KConfigDialog::settingsChanged, this, &SettingsManager::settingsChanged);
    dialog->show();
}
