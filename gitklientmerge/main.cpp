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

// application header
#include "gitklientmergewindow.h"
#include "gitklientdebug.h"

// KF headers
#include <KCrash>
#include <KDBusService>
#include <KAboutData>
#include <KLocalizedString>

// Qt headers
#include <QApplication>
#include <QCommandLineParser>
#include <QDir>
#include <QIcon>
#include <QLoggingCategory>

QString concatPath(const QString &dir, const QString &file)
{
    QString f;
    if (file.startsWith("./"))
        f = file.mid(2);
    else
        f = file;

    QString ret;
    if (dir.endsWith("/"))
        ret = dir + f;
    else
        ret = dir + "/" + f;
    if (QFile::exists(ret))
        qDebug() << "[EXISTS]" << ret;
    else
        qDebug() << "[NOT EXISTS]" << ret;
    return ret;
}
int main(int argc, char **argv)
{
    QApplication application(argc, argv);

    KLocalizedString::setApplicationDomain("gitklientmerge");
    KCrash::initialize();

    KAboutData aboutData( QStringLiteral("gitklientmerge"),
                          i18n("GitKlient Merge"),
                          QStringLiteral("0.1"),
                          i18n("Git Klient Merge Tool"),
                          KAboutLicense::GPL,
                          i18n("Copyright 2021, Hamed Masafi <hamed.masafi@gmail.com>"));

    aboutData.addAuthor(i18n("Hamed Masafi"),i18n("Author"), QStringLiteral("hamed.masafi@gmail.com"));
    aboutData.setOrganizationDomain("kde.org");
    aboutData.setDesktopFileName(QStringLiteral("org.kde.gitklientmerge"));

    KAboutData::setApplicationData(aboutData);
    application.setWindowIcon(QIcon::fromTheme(QStringLiteral("gitklient")));

    QCommandLineParser parser;
    aboutData.setupCommandLine(&parser);

    parser.process(application);
    aboutData.processCommandLine(&parser);

    KDBusService appDBusService(KDBusService::Multiple | KDBusService::NoExitOnFailure);

    GitKlientMergeWindow *window;

    if (application.arguments().size() == 5) {
        QDir d;
        window = new GitKlientMergeWindow(GitKlientMergeWindow::MergeByParams);
        window->setFilePathBase(concatPath(d.currentPath(), application.arguments().at(1)));
        window->setFilePathLocal(concatPath(d.currentPath(), application.arguments().at(2)));
        window->setFilePathRemote(concatPath(d.currentPath(), application.arguments().at(3)));
        window->setFilePathResult(concatPath(d.currentPath(), application.arguments().at(4)));
        window->load();
    } else {
        window = new GitKlientMergeWindow(GitKlientMergeWindow::NoParams);
    }
    window->show();

    return application.exec();
}
