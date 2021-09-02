#include "diffwindow.h"

#include <KActionCollection>
#include <KLocalizedString>
#include <QDockWidget>

#include <QApplication>
#include <QTreeView>

#include "dialogs/diffopendialog.h"
#include "git/gitmanager.h"
#include "models/difftreemodel.h"
#include "widgets/codeeditor.h"
#include "widgets/diffwidget.h"
#include "widgets/editactionsmapper.h"

void DiffWindow::init()
{
    auto mapper = new EditActionsMapper;
    mapper->init(actionCollection());

    initActions();
    setupGUI(StandardWindowOption::Default, "/doc/dev/gitklient/gitklient/gitklientdiffui.rc");

    _diffWidget = new DiffWidget(this);
    setCentralWidget(_diffWidget);

    mapper->addTextEdit(_diffWidget->oldCodeEditor());
    mapper->addTextEdit(_diffWidget->newCodeEditor());
    setWindowTitle(i18n("GitKlient Diff[*]"));

    auto dock = new QDockWidget(this);
    dock->setObjectName("treeViewDock");

    _treeView = new QTreeView(this);
    dock->setWidget(_treeView);
    addDockWidget(Qt::LeftDockWidgetArea, dock);
}

DiffWindow::DiffWindow() : KXmlGuiWindow()
{
    init();
}

DiffWindow::DiffWindow(const Git::File &oldFile, const Git::File &newFile)
    : KXmlGuiWindow(), _oldFile(oldFile), _newFile(newFile)
{
    init();

    _diffWidget->setOldFile(std::move(oldFile));
    _diffWidget->setNewFile(std::move(newFile));
    _diffWidget->compare();
}

DiffWindow::DiffWindow(const QString &oldBranch, const QString &newBranch)
    : KXmlGuiWindow(), _oldBranch(oldBranch), _newBranch(newBranch)
{
    init();

    _diffModel = new DiffTreeModel;
    auto diffs = Git::Manager::instance()->diffBranches(oldBranch, newBranch);

    for (auto &f: diffs)
        _diffModel->addFile(f);
    _treeView->setModel(_diffModel);
}

void DiffWindow::fileOpen()
{
    DiffOpenDialog d(this);
    d.exec();
}

void DiffWindow::initActions()
{
    KActionCollection* actionCollection = this->actionCollection();

    auto viewHiddenCharsAction = actionCollection->addAction(QStringLiteral("view_hidden_chars"));
    viewHiddenCharsAction->setText(i18n("View hidden chars..."));
    viewHiddenCharsAction->setCheckable(true);
    connect(viewHiddenCharsAction, &QAction::triggered, _diffWidget, &DiffWidget::showHiddenChars);

    KStandardAction::quit(this, &QWidget::close, actionCollection);
    KStandardAction::preferences(this, SLOT(settingsConfigure()), actionCollection);
    KStandardAction::open(this, &DiffWindow::fileOpen, actionCollection);
}
