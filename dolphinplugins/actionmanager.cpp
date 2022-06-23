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

    auto items = fileItemInfos.items();
    if (items.size() == 1) {
        KFileItem &item = items.first();
        auto path = item.url().toLocalFile();
        FileStatus::Status status;
        if (item.isFile())
            status = _cache.fileStatus(path);
        else
            status = _cache.pathStatus(path);

        if (status == FileStatus::NoGit) {
            if (fileItemInfos.isDirectory()) {
                addMenuToNonGitFile(menu, path);
            }
        } else {
            addMenuToGitFile(menu, path, fileItemInfos.isFile(), status);
        }

    }

    mainAction->setMenu(menu);
/*
    auto openAction = new QAction;
    openAction->setText("Open git klient");
    openAction->setIcon(QIcon::fromTheme("gitklient"));
*/
    return QList<QAction *>() << /*openAction << */mainAction;
}

QString ActionManager::getCommonPart(const KFileItemList &fileItems)
{
    if (!fileItems.size())
        return QString();

    QStringList list;
    for (auto const &i: fileItems)
        list.append(i.url().toLocalFile());

    QString root = list.front();
    for(QStringList::const_iterator it = list.cbegin(); it != list.cend(); ++it)
    {
        if (root.length() > it->length())
        {
            root.truncate(it->length());
        }

        for(int i = 0; i < root.length(); ++i)
        {
            if (root.at(i) != it->at(i))
            {
                root.truncate(i);
                break;
            }
        }
    }

    return root;
}

void ActionManager::addMenuToNonGitFile(QMenu *menu, const QString &path)
{
    addMenu(menu, i18n("Clone"), {"clone", path});
    addMenu(menu, i18n("Init"), {"init", path});
}

void ActionManager::addMenuToGitFile(QMenu *menu, const QString &path, bool isFile, const FileStatus::Status &status)
{
    addMenu(menu, i18n("Open"), {path});
    addMenu(menu, i18n("Pull"), {"pull", path});
    addMenu(menu, i18n("Push"), {"push", path});
    addMenu(menu, i18n("Modifications"), {"changes", path});
    addMenu(menu, i18n("Diff"), {"diff", path});
    addMenu(menu, i18n("Ignore file"), {"ignore", path});
    if (isFile) {
        addMenu(menu, i18n("History"), {"history", path});
        addMenu(menu, i18n("Blame"), {"blame", path});
        if (status == FileStatus::Untracked) {
            addMenu(menu, i18n("Add"), {"add", path});
        } else {
            addMenu(menu, i18n("Remove"), {"remove", path});
        }
    }
    addMenu(menu, i18n("Create tag"), {"create-tag", path});

}

K_PLUGIN_FACTORY_WITH_JSON(GitKlientPluginActionFactory,
                           "gitklientitemaction.json",
                           registerPlugin<ActionManager>();)
#include "actionmanager.moc"
