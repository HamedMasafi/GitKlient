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

#include "core/appmainwindow.h"

#include "GitKlientSettings.h"

class MultiPageWidget;
class WidgetBase;
class QLabel;

namespace Git {
class Manager;
}
/**
 * This class serves as the main window for gitklient.  It handles the
 * menus, toolbars and status bars.
 *
 * @short Main window class
 * @author Hamed Masafi <hamed.masafi@gmail.com>
 * @version 0.1
 */
class AppWindow : public AppMainWindow
{
    Q_OBJECT
    Git::Manager *_git;

public:
    /**
     * Default Constructor
     */
    AppWindow();
    AppWindow(const QString &path);

    /**
     * Default Destructor
     */
    ~AppWindow() override;

    static AppWindow *instance();

private Q_SLOTS:
    void git_pathChanged();
    void repoStatus();
    void initRepo();
    void openRepo();
    void recentActionTriggered();
    void commitPushAction();
    void pull();
    void fetch();
    void showBranchesStatus();
    void clone();
    void diffBranches();
    void search();
    void repoSettings();
    void repoSwitch();
    void repoDiffTree();
    void merge();

private:
    QAction *recentAction;
    MultiPageWidget *_mainWidget;
    QList<WidgetBase*> _baseWidgets;
    QLabel *_statusCurrentBranchLabel;

    void initActions();
    void initRecentFiles(const QString &newItem = QString());
    void loadRemotes();

    template <class T>
    void addPage(const QString &actionName);
    void init();
};

#endif // GITKLIENTWINDOW_H
