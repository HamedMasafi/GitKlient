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
#include "common/commandargsparser.h"
#include "dialogs/diffdialog.h"
#include "dialogs/pulldialog.h"
#include "dialogs/runnerdialog.h"
#include "dialogs/changedfilesdialog.h"
#include "git/gitfile.h"
#include "git/gitmanager.h"
#include "diffwindow.h"
#include "gitklientdebug.h"
#include "gitklientwindow.h"

// KF headers
#include <KAboutData>
#include <KCrash>
#include <KDBusService>
#include <KLocalizedString>

// Qt headers
#include <QApplication>
#include <QCommandLineParser>
#include <QFileInfo>
#include <QIcon>
#include <QLoggingCategory>

enum ParseReturnType {
    ExecApp,
    ReturnCode
};

struct ArgParserReturn {
    ParseReturnType type;
    int code{0};

    ArgParserReturn(int code) : type{ReturnCode}, code{code} {}
    ArgParserReturn(ParseReturnType type) : type{type}, code{0} {}
};

ArgParserReturn argsWidget() {
    auto git = Git::Manager::instance();
    CommandArgsParser p;
    p.add("pull", "pull <path>");
    p.add("changes", "changes <path>");
    p.add("diff", "diff <file>");
    p.add("merge", "merge <base> <local> <remote> <result>");
    p.add("diff1", "diff");

    auto key = p.checkAll();

    if (key == "pull"){
        git->setPath(p.param("path"));
        PullDialog d;
        if (d.exec() == QDialog::Accepted) {
            RunnerDialog r;
            auto branch = git->currentBranch();
            r.run({"pull", "origin", branch});
            r.exec();
        }
        return 0;
    } else if (key == "diff") {
        auto filePath = p.param("file");
        QFileInfo fi(filePath);
        git->setPath(fi.absolutePath());
        Git::File original{git->currentBranch(), filePath.replace(git->path() + "/", "")};
        Git::File changed{fi.absoluteFilePath()};
        qDebug() << "[DIFF]" << git->currentBranch() + ":" + filePath.replace(git->path() + "/", "")
                 << filePath;
        DiffDialog d(original, changed);
        d.exec();
        return 0;
    } else if (key == "changes") {
        git->setPath(p.param("path"));
        ChangedFilesDialog d;
        d.exec();
        return 0;
    } else if (key == "diff1") {
        auto d = new DiffWindow;
        d->show();
        return ExecApp;
    } else {
        GitKlientWindow *window = new GitKlientWindow;
        window->show();
        return ExecApp;
    }
    return 0;
}
int main(int argc, char **argv)
{
    QApplication application(argc, argv);

    KLocalizedString::setApplicationDomain("gitklient");
    KCrash::initialize();

    KAboutData aboutData( QStringLiteral("gitklient"),
                          i18n("GitKlient"),
                          QStringLiteral("0.1"),
                          i18n("A Simple Application written with KDE Frameworks"),
                          KAboutLicense::GPL,
                          i18n("Copyright 2021, Hamed Masafi <hamed.masafi@gmail.com>"));

    aboutData.addAuthor(i18n("Hamed Masafi"),i18n("Author"), QStringLiteral("hamed.masafi@gmail.com"));
    aboutData.setOrganizationDomain("kde.org");
    aboutData.setDesktopFileName(QStringLiteral("org.kde.gitklient"));

    KAboutData::setApplicationData(aboutData);
    application.setWindowIcon(QIcon::fromTheme(QStringLiteral("gitklient")));

    QCommandLineParser parser;
    aboutData.setupCommandLine(&parser);

    parser.process(application);
    aboutData.processCommandLine(&parser);

    KDBusService appDBusService(KDBusService::Multiple | KDBusService::NoExitOnFailure);
    auto w = argsWidget();
    if (w.type == ExecApp) {
        return application.exec();
    }
    return w.code;



}
