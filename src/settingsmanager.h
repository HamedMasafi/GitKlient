#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include "ui_settingsBase.h"
#include "ui_diffsettings.h"

class SettingsManager : public QObject
{
    Q_OBJECT

public:
    SettingsManager();
    static SettingsManager *instance();

private slots:
    void settingsChanged();

public slots:
    void show();

private:
    Ui::settingsBase settingsBase;
    Ui::DiffSettings diffSettings;
};

#endif // SETTINGSMANAGER_H
