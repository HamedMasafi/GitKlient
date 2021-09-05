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

#ifndef GITKLIENTWINDOW_H
#define GITKLIENTWINDOW_H

#include <KXmlGuiWindow>

#include "ui_settingsBase.h"
#include "ui_diffsettings.h"
#include "GitKlientSettings.h"

class GitKlientView;
class MultiPageWidget;
class MainWidget;
/**
 * This class serves as the main window for gitklient.  It handles the
 * menus, toolbars and status bars.
 *
 * @short Main window class
 * @author Hamed Masafi <hamed.masafi@gmail.com>
 * @version 0.1
 */
class GitKlientWindow : public KXmlGuiWindow
{
    Q_OBJECT
public:
    /**
     * Default Constructor
     */
    GitKlientWindow();

    /**
     * Default Destructor
     */
    ~GitKlientWindow() override;

private Q_SLOTS:
    void settingsConfigure();
    void browseBranch();
    void repoStatus();
    void openRepo();
    void recentActionTriggered();
    void commitPushAction();
    void deleteBranch();
    void pull();
    void showBranchesStatus();
    void clone();
    void diffBranches();

private:
    Ui::settingsBase settingsBase;
    Ui::DiffSettings diffSettings;
    QAction *recentAction;
    MultiPageWidget *_mainWidget;

    void initActions();
    void initRecentFiles(const QString &newItem = QString());
    void loadRemotes();

    template <class T>
    void addPage(const QString &actionName);
};

#endif // GITKLIENTWINDOW_H
