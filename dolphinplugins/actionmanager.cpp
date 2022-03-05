#include "actionmanager.h"

#include "../src/git/gitmanager.h"
#include <KFileItem>
#include <KFileItemListProperties>
#include <KIOWidgets/KAbstractFileItemActionPlugin>
#include <KLocalizedString>
#include <KPluginFactory>
#include <KPluginLoader>
#include <KProcess>
#include <KService>
#include <QApplication>
#include <QDebug>
#include <QtWidgets/QAction>
#include <QtWidgets/QMenu>

using namespace Git;

ActionManager::ActionManager(QObject *parent, const QList<QVariant> &) : KAbstractFileItemActionPlugin(parent)
{

}

void ActionManager::addMenu(QMenu *menu, const QString &title, const QStringList &args)
{
    auto action = menu->addAction(title);
    connect(action, &QAction::triggered, [args]() {
        qDebug() << "[RUN]" << args;
        KProcess::startDetached("gitklient", args);
    });
}

QList<QAction *> ActionManager::actions(const KFileItemListProperties &fileItemInfos,
                                        QWidget *parentWidget)
{
    Q_UNUSED(parentWidget);

    auto mainAction = new QAction;
    mainAction->setText("Git Klient");
    mainAction->setIcon(QIcon::fromTheme("gitklient"));

    auto menu = new QMenu;
    /*QString path;
        int index{0};
        while (true) {

    for (auto &f: fileItemInfos.items()) {
        menu->addAction(f.text());
    }

}*/

    if (fileItemInfos.items().size() == 1) {
        KFileItem &item = fileItemInfos.items().first();
        auto path = item.url().toLocalFile();
        FileStatus::Status status;
        if (item.isFile())
            status = _cache.fileStatus(path);
        else
            status = _cache.pathStatus(path);

        if (status == FileStatus::NoGit) {
            addMenu(menu, i18n("Clone"), QStringList() << "--clone");
            addMenu(menu, i18n("Init"), QStringList() << "--init");
        } else {
            addMenu(menu, i18n("Pull"), {"pull", path});
            addMenu(menu, i18n("Push"), {"push", path});
            addMenu(menu, i18n("Modifications"), {"changes", path});
            if (fileItemInfos.isFile()) {
                addMenu(menu, i18n("Diff"), {"diff", path});
                addMenu(menu, i18n("History"), {"history", path});
                addMenu(menu, i18n("Blame"), {"blame", path});
            }
        }

    } else {
        menu->addAction(QStringLiteral("Is dir: %1").arg(fileItemInfos.isDirectory()));
        menu->addAction(QStringLiteral("Is local: %1").arg(fileItemInfos.isLocal()));

        menu->addAction(QStringLiteral("Count: %1").arg(fileItemInfos.items().size()));
        for (auto &f : fileItemInfos.items()) {
            menu->addAction(f.text());
        }
    }

    mainAction->setMenu(menu);
    return QList<QAction *>() << mainAction;
}

K_PLUGIN_FACTORY_WITH_JSON(GitKlientPluginActionFactory,
                           "gitklientitemaction.json",
                           registerPlugin<ActionManager>();)
#include "actionmanager.moc"
