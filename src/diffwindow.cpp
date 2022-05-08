#include "diffwindow.h"

#include <KActionCollection>
#include <KLocalizedString>

#include <QApplication>
#include <QDebug>
#include <QDockWidget>
#include <QTreeView>

#include "dialogs/diffopendialog.h"
#include "git/gitmanager.h"
#include "models/difftreemodel.h"
#include "models/filesmodel.h"
#include "settingsmanager.h"
#include "widgets/codeeditor.h"
#include "widgets/difftreeview.h"
#include "widgets/diffwidget.h"
#include "widgets/editactionsmapper.h"

void DiffWindow::init(bool showSideBar)
{
    auto mapper = new EditActionsMapper;
    _diffWidget = new DiffWidget(this);

    mapper->init(actionCollection());

    initActions();
    setupGUI(StandardWindowOption::Default, "gitklientdiffui.rc");

    setCentralWidget(_diffWidget);

    mapper->addTextEdit(_diffWidget->oldCodeEditor());
    mapper->addTextEdit(_diffWidget->newCodeEditor());
    setWindowTitle(i18n("GitKlient Diff[*]"));

    if (showSideBar) {
        auto dock = new QDockWidget(this);
        dock->setWindowTitle(i18n("Tree"));
        dock->setObjectName("treeViewDock");

        _treeView = new DiffTreeView(this);
        connect(_treeView, &DiffTreeView::fileSelected, this, &DiffWindow::on_treeView_fileSelected);
        dock->setWidget(_treeView);
        addDockWidget(Qt::LeftDockWidgetArea, dock);

        _filesModel = new FilesModel(this);
        _diffModel = new DiffTreeModel(this);
        _treeView->setDiffModel(_diffModel, _filesModel);
    }
}

DiffWindow::DiffWindow() : AppMainWindow()
{
    init(true);
}

DiffWindow::DiffWindow(Git::Manager *git) : AppMainWindow()
{
    init(true);

    _oldBranch = git->currentBranch();
    auto diffs = git->diffBranch(_oldBranch);

    for (auto &f: diffs) {
        _diffModel->addFile(f);
        _filesModel->append(f.name());
    }
    _leftStorage = Git;
    _rightStorage =  FileSystem;
    _rightDir = git->path();
    _diffModel->sortItems();
}

DiffWindow::DiffWindow(const Git::File &oldFile, const Git::File &newFile)
    : AppMainWindow(), _oldFile(oldFile), _newFile(newFile)
{
    init(false);

    _diffWidget->setOldFile(std::move(oldFile));
    _diffWidget->setNewFile(std::move(newFile));
    _diffWidget->compare();
}

DiffWindow::DiffWindow(Git::Manager *git, const QString &oldBranch, const QString &newBranch)
    : AppMainWindow(), _oldBranch(oldBranch), _newBranch(newBranch)
{
    init(true);

    auto diffs = git->diffBranches(oldBranch, newBranch);

    for (auto &f: diffs) {
        _diffModel->addFile(f);
//        qDebug() << f.name() << f.status();
        _filesModel->append(f.name());
    }
    _leftStorage = _rightStorage = Git;
    _diffModel->sortItems();
}

DiffWindow::DiffWindow(const QString &oldDir, const QString &newDir)
{
    init(true);

    _leftDir = oldDir;
    _rightDir = newDir;
    compareDirs();

    _leftStorage = _rightStorage = FileSystem;
}

void DiffWindow::fileOpen()
{
    DiffOpenDialog d(this);
    if (d.exec() != QDialog::Accepted)
        return;

    _leftStorage = _rightStorage = FileSystem;
    if (d.mode() == DiffOpenDialog::Dirs) {
        _leftDir = d.oldDir();
        _rightDir = d.newDir();
        compareDirs();
    } else {
        _diffWidget->setOldFile({d.oldFile()});
        _diffWidget->setNewFile({d.newFile()});
        _diffWidget->compare();
    }
}

void DiffWindow::settings()
{
    SettingsManager::instance()->exec(this);
}

void DiffWindow::on_treeView_fileSelected(const QString &file)
{
    switch (_leftStorage) {
    case FileSystem:
        _diffWidget->setOldFile({_leftDir + "/" + file});
        break;
    case Git:
        _diffWidget->setOldFile({_oldBranch, file});
        break;
    case NoStorage:
        return;
    }
    switch (_rightStorage) {
    case FileSystem:
        _diffWidget->setNewFile({_rightDir + "/" + file});
        break;
    case Git:
        _diffWidget->setNewFile({_newBranch, file});
        break;
    case NoStorage:
        return;
    }
    _diffWidget->compare();
}

QString diffTypeText(const Diff::DiffType type)
{
    switch (type) {
    case Diff::DiffType::Unchanged: return i18n("Unchanged");
    case Diff::DiffType::Added: return i18n("Added");
    case Diff::DiffType::Removed: return i18n("Removed");
    case Diff::DiffType::Modified: return i18n("Modified");
    }
    return QString();
}

void DiffWindow::compareDirs()
{
    auto map = Diff::diffDirs(_leftDir, _rightDir);
    for (auto i = map.begin(); i != map.end(); ++i) {
        _diffModel->addFile(i.key(), i.value());
    }
    _diffModel->emitAll();
}

void DiffWindow::initActions()
{
    KActionCollection* actionCollection = this->actionCollection();

    auto viewHiddenCharsAction = actionCollection->addAction(QStringLiteral("view_hidden_chars"));
    viewHiddenCharsAction->setText(i18n("View hidden chars..."));
    viewHiddenCharsAction->setCheckable(true);
    connect(viewHiddenCharsAction, &QAction::triggered, _diffWidget, &DiffWidget::showHiddenChars);

//    auto viewSameSizeBlocksAction = actionCollection->addAction(QStringLiteral(
//                                                                    "view_same_size_blocks"),
//                                                                _diffWidget,
//                                                                &DiffWidget::showSameSize);
//    viewSameSizeBlocksAction->setText(i18n("Same size blocks"));
//    viewSameSizeBlocksAction->setCheckable(true);

    auto viewFilesInfo = actionCollection->addAction(QStringLiteral("view_files_info"),
                                                     _diffWidget,
                                                     &DiffWidget::showFilesInfo);
    viewFilesInfo->setText(i18n("Show files names"));
    viewFilesInfo->setCheckable(true);
    viewFilesInfo->setChecked(true);

    KStandardAction::quit(this, &QWidget::close, actionCollection);
//    KStandardAction::preferences(this, &DiffWindow::settings, actionCollection);
    KStandardAction::open(this, &DiffWindow::fileOpen, actionCollection);
}
