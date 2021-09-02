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

#ifndef GITKLIENTVIEW_H
#define GITKLIENTVIEW_H

#include "ui_gitklientViewBase.h"


/**
 * This class serves as the main window for gitklient.  It handles the
 * menus, toolbars and status bars.
 *
 * @short Main window class
 * @author Hamed Masafi <hamed.masafi@gmail.com>
 * @version 0.1
 */
class GitKlientView : public QWidget
{
    Q_OBJECT
public:
    /**
     * Default Constructor
     */
    explicit GitKlientView(QWidget *parent);

    /**
     * Default Destructor
     */
    ~GitKlientView() override;

public Q_SLOTS:

private:
    // this is the name of the root widget inside our Ui file
    // you can rename it in designer and then change it here
    Ui::gitklientViewBase m_ui;
};

#endif // GITKLIENTVIEW_H
