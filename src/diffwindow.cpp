#include "diffwindow.h"

#include <KActionCollection>
#include <KLocalizedString>

#include <QDockWidget>
#include <QApplication>
#include <QTreeView>
#include <QDebug>

#include "dialogs/diffopendialog.h"
#include "git/gitmanager.h"
#include "models/difftreemodel.h"
#include "widgets/codeeditor.h"
#include "widgets/difftreeview.h"
#include "widgets/diffwidget.h"
#include "widgets/editactionsmapper.h"

#include <models/filesmodel.h>

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

DiffWindow::DiffWindow() : KXmlGuiWindow()
{
    init(true);
}

DiffWindow::DiffWindow(const Git::File &oldFile, const Git::File &newFile)
    : KXmlGuiWindow(), _oldFile(oldFile), _newFile(newFile)
{
    init(false);

    _diffWidget->setOldFile(std::move(oldFile));
    _diffWidget->setNewFile(std::move(newFile));
    _diffWidget->compare();
}

DiffWindow::DiffWindow(const QString &oldBranch, const QString &newBranch)
    : KXmlGuiWindow(), _oldBranch(oldBranch), _newBranch(newBranch)
{
    init(true);

    auto diffs = Git::Manager::instance()->diffBranches(oldBranch, newBranch);

    for (auto &f: diffs) {
        _diffModel->addFile(f);
        _filesModel->append(f.name());
    }
    _storage = Git;
}

void DiffWindow::fileOpen()
{
    DiffOpenDialog d(this);
    if (d.exec() != QDialog::Accepted)
        return;

    _storage = FileSystem;
    if (d.mode() == DiffOpenDialog::Dirs) {
        _oldDir = d.oldDir();
        _newDir = d.newDir();
        compareDirs();
    } else {
    }
}

void DiffWindow::on_treeView_fileSelected(const QString &file)
{
    if (_storage == FileSystem) {
        _diffWidget->setOldFile({_oldDir + "/" + file});
        _diffWidget->setNewFile({_newDir + "/" + file});
    } else if (_storage == Git) {
        _diffWidget->setOldFile({_oldBranch, file});
        _diffWidget->setNewFile({_newBranch, file});
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
    auto map = Diff::diffDirs(_oldDir, _newDir);
    for (auto i = map.begin(); i != map.end(); ++i) {
        _diffModel->addFile(i.key(), i.value());
        qDebug() << i.key() << diffTypeText(i.value());
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

    auto viewSameSizeBlocksAction = actionCollection->addAction(QStringLiteral("view_same_size_blocks"));
    viewSameSizeBlocksAction->setText(i18n("Same size blocks"));
    viewSameSizeBlocksAction->setCheckable(true);

    auto viewFilesInfo = actionCollection->addAction(QStringLiteral("view_files_info"),
                                                     _diffWidget,
                                                     &DiffWidget::showFilesInfo);
    viewFilesInfo->setText(i18n("Show files names"));
    viewFilesInfo->setCheckable(true);
    viewFilesInfo->setChecked(true);

    KStandardAction::quit(this, &QWidget::close, actionCollection);
//    KStandardAction::preferences(this, SLOT(settingsConfigure()), actionCollection);
    KStandardAction::open(this, &DiffWindow::fileOpen, actionCollection);
}
