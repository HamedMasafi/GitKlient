#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QObject>
#include "ui_settingspagebase.h"
#include "ui_settingspagediff.h"
#include "ui_settingspagegit.h"

class SettingsManager : public QObject
{
    Q_OBJECT

public:
    SettingsManager();
    static SettingsManager *instance();
    void exec(QWidget *parentWidget);


private slots:
    void settingsChanged();

public slots:
    void show();

private:
    Ui::SettingsPageBase pageBase{};
    Ui::SettingsPageDiff pageDiff{};
    Ui::SettingsPageGit pageGit{};
    QWidget * createBasePage();
    QWidget *createGitPage();
    QWidget *createDiffPage();
};

#endif // SETTINGSMANAGER_H
