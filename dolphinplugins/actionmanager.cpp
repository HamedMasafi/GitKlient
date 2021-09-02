
#include "git/gitmanager.h"
#include <KFileItem>
#include <KFileItemListProperties>
#include <KIOWidgets/KAbstractFileItemActionPlugin>
#include <KPluginFactory>
#include <KPluginLoader>
#include <KProcess>
#include <KService>
#include <QApplication>
#include <QDebug>
#include <QtWidgets/QAction>
#include <QtWidgets/QMenu>

using namespace Git;

class ActionManager : public KAbstractFileItemActionPlugin
{
    Q_OBJECT
    QMap<QString,QMap<QString, Git::Manager::ChangeStatus>> _cache;

public:
    explicit ActionManager(QObject *parent, const QList<QVariant>&)
            : KAbstractFileItemActionPlugin(parent)
    {}

    Git::Manager::ChangeStatus checkCache(const QString &path)
    {
        for (auto i = _cache.begin();i!=_cache.end(); ++i)
            if (path.startsWith(i.key())) {
                return i.value().value(path, Git::Manager::Unknown);
            }

        return Git::Manager::Unknown;
    };

    void get(const QString &path)
    {
        auto g = new Git::Manager;
        auto statuses = g->changedFiles();
        _cache.insert(path, statuses);
    }
    void addMenu(QMenu *menu, const QString &title, const QStringList &args)
    {
        auto action = menu->addAction(title);
        connect(action, &QAction::triggered, [args]() {
            qDebug() << "[RUN]" << args;
            KProcess::startDetached("gitklient", args);
        });
    }
    QList<QAction*> actions(const KFileItemListProperties& fileItemInfos, QWidget* parentWidget) Q_DECL_OVERRIDE
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
            auto path = fileItemInfos.items().first().url().toLocalFile();
            Git::Manager g{path};
            if (!g.isValid()) {
                addMenu(menu, "Clone", QStringList() << "--clone");
                addMenu(menu, "Init", QStringList() << "--init");
            } else {
                addMenu(menu, "Pull", {"pull", path});
                addMenu(menu, "Modifications", {"changes", path});
                if (fileItemInfos.isFile())
                    addMenu(menu, "Diff", {"diff", path});
            }
                
        } else {

            menu->addAction(QStringLiteral("Is dir: %1").arg(fileItemInfos.isDirectory()));
            menu->addAction(QStringLiteral("Is local: %1").arg(fileItemInfos.isLocal()));

            menu->addAction(QStringLiteral("Count: %1").arg(fileItemInfos.items().size()));
            for (auto &f: fileItemInfos.items()) {
                menu->addAction(f.text());
            }
        }
            
        mainAction->setMenu(menu);
        return QList<QAction*>() << mainAction;
    }
};

K_PLUGIN_FACTORY_WITH_JSON(HamedPluginActionFactory,
                           "kdegititemaction.json",
                           registerPlugin<ActionManager>();)

#include "actionmanager.moc"
