#include "settingsmanager.h"

#include "GitKlientSettings.h"
#include "gitklientwindow.h"

#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
#include <QCalendar>
#endif

#include <KConfigDialog>

#include <QAction>
#include <QWidget>

SettingsManager::SettingsManager()
{

}

SettingsManager *SettingsManager::instance()
{
    static SettingsManager *instance = new SettingsManager;
    return instance;
}

void SettingsManager::settingsChanged()
{
    qDebug() << GitKlientSettings::calendarType() << settingsBase.kcfg_calendarType->currentText();
    GitKlientSettings::setCalendarType(settingsBase.kcfg_calendarType->currentText());
}

void SettingsManager::show()
{
    exec(GitKlientWindow::instance());
}

void SettingsManager::exec(QWidget *parentWidget)
{
    if (KConfigDialog::showDialog(QStringLiteral("settings"))) {
        return;
    }

    KConfigDialog *dialog = new KConfigDialog(parentWidget,
                                              QStringLiteral("settings"),
                                              GitKlientSettings::self());

    QWidget *generalSettingsPage = new QWidget;
    settingsBase.setupUi(generalSettingsPage);

#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    settingsBase.kcfg_calendarType->addItems(QCalendar::availableCalendars());
    settingsBase.kcfg_calendarType->setCurrentText(GitKlientSettings::calendarType());
#else
    settingsBase.labelDefaultCalendar->hide();
    settingsBase.kcfg_calendarType->hide();
#endif

    dialog->addPage(generalSettingsPage, i18n("General"), QStringLiteral("package_setting"));

    auto diffSettingsPage = new QWidget;
    diffSettings.setupUi(diffSettingsPage);
    dialog->addPage(diffSettingsPage, i18n("Diff"), QStringLiteral("package_setting"));

    connect(dialog, &KConfigDialog::settingsChanged, this, &SettingsManager::settingsChanged);
    dialog->show();
}
