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

#include "mergewindow.h"

#include "GitKlientSettings.h"
#include "dialogs/mergecloseeventdialog.h"
#include "dialogs/mergeopenfilesdialog.h"
#include "diff/segmentsmapper.h"
#include "settings/settingsmanager.h"
#include "widgets/codeeditor.h"
#include "widgets/editactionsmapper.h"

#include <KActionCollection>
#include <KConfigDialog>
#include <KLocalizedString>
#include <KMessageBox>

#include <QLabel>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QMenu>
#include <QScrollBar>
#include <QStatusBar>
#include <QTextBlock>
#include <QTextEdit>


bool isEmpty(const QStringList &list)
{
    if (!list.size())
        return true;

    for (auto &s : list)
        if (!s.trimmed().isEmpty())
            return false;
    return true;
}
void compare(QTextEdit *e1, QTextEdit *e2)
{
    auto m = qMin(e1->document()->blockCount(), e2->document()->blockCount());
    for (int i = 0; i < m; ++i) {
        auto block1 = e1->document()->findBlock(i);
        auto block2 = e2->document()->findBlock(i);
        if (block1.text() != block2.text()) {
            //            block1.blockFormat()
        }
    }
}
QStringList readFile(const QString &filePath)
{
//    QStringList buffer;
    QFile f{filePath};
    if (!f.open(QIODevice::ReadOnly))
        return QStringList();

//    while (!f.atEnd()) {
//        buffer << f.readLine();
//    }
    auto buf = QString(f.readAll()).split('\n');
    f.close();
    return buf;
}

GitKlientMergeWindow::GitKlientMergeWindow(Mode mode, QWidget *parent) : AppMainWindow(parent)
{
    Q_UNUSED(mode)

    initActions();
    auto mapper = new EditActionsMapper;
    mapper->init(actionCollection());
    setupGUI(Default, "gitklientmergeui.rc");
    auto w = new QWidget(this);
    m_ui.setupUi(w);
    setCentralWidget(w);


    mapper->addTextEdit(m_ui.plainTextEditMine);
    mapper->addTextEdit(m_ui.plainTextEditTheir);
    mapper->addTextEdit(m_ui.plainTextEditResult);


    _mapper = new SegmentsMapper;

    _mapper->addEditor(m_ui.plainTextEditBase);
    _mapper->addEditor(m_ui.plainTextEditMine);
    _mapper->addEditor(m_ui.plainTextEditTheir);
    _mapper->addEditor(m_ui.plainTextEditResult);

    m_ui.plainTextEditMine->setContextMenuPolicy(Qt::CustomContextMenu);
    m_ui.plainTextEditTheir->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(m_ui.plainTextEditMine,
            &CodeEditor::customContextMenuRequested,
            this,
            &GitKlientMergeWindow::codeEditors_customContextMenuRequested);

    connect(m_ui.plainTextEditTheir,
            &CodeEditor::customContextMenuRequested,
            this,
            &GitKlientMergeWindow::codeEditors_customContextMenuRequested);

    /*auto actions = QList<QAction*> ()
                   << actionKeepThis
                   << actionKeepThis
                   << actionKeepThisBeforeOther
                   << actionKeepThisAfterOther;*/

    //    m_ui.plainTextEditMine->insertActions(nullptr, actions);

    _conflictsLabel = new QLabel(this);
    statusBar()->addPermanentWidget(_conflictsLabel);
}

GitKlientMergeWindow::~GitKlientMergeWindow() {}

void GitKlientMergeWindow::load()
{
    m_ui.plainTextEditMine->clear();
    m_ui.plainTextEditTheir->clear();
    m_ui.plainTextEditResult->clear();
    m_ui.plainTextEditBase->clear();

    auto baseList = readFile(_filePathBase);
    auto localList = readFile(_filePathLocal);
    auto remoteList = readFile(_filePathRemote);

    m_ui.plainTextEditBase->setHighlighting(_filePathBase);
    m_ui.plainTextEditMine->setHighlighting(_filePathLocal);
    m_ui.plainTextEditTheir->setHighlighting(_filePathRemote);
    m_ui.plainTextEditResult->setHighlighting(_filePathResult);

    _diffs = Diff::diff3(baseList, localList, remoteList);
    _mapper->setSegments(_diffs);
    QList<Diff::Segment *> segments;
    for (auto &s : _diffs) {
        segments.append(s);
    }
    m_ui.widgetSegmentsConnector->setSegments(segments);
    m_ui.widgetSegmentsConnector->setLeft(m_ui.plainTextEditMine);
    m_ui.widgetSegmentsConnector->setRight(m_ui.plainTextEditTheir);
    m_ui.widgetSegmentsConnector->hide();

    //    m_ui.plainTextEditResult->setVisible(false);

    for (auto &d : _diffs) {
        switch (d->type) {
        case Diff::SegmentType::SameOnBoth: {
            m_ui.plainTextEditMine->append(d->base, CodeEditor::Unchanged, d);
            m_ui.plainTextEditTheir->append(d->base, CodeEditor::Unchanged, d);
            m_ui.plainTextEditBase->append(d->base, CodeEditor::Unchanged, d);
            d->mergeType = Diff::KeepLocal;
            break;
        }

        case Diff::SegmentType::OnlyOnRight:
            m_ui.plainTextEditMine->append(d->local, CodeEditor::Removed, d);
            m_ui.plainTextEditTheir->append(d->remote, CodeEditor::Added, d);
            m_ui.plainTextEditBase->append(d->base, CodeEditor::Edited, d);
            d->mergeType = Diff::KeepRemote;
            break;
        case Diff::SegmentType::OnlyOnLeft:
            m_ui.plainTextEditMine->append(d->local, CodeEditor::Added, d);
            m_ui.plainTextEditTheir->append(d->remote, CodeEditor::Removed, d);
            m_ui.plainTextEditBase->append(d->base, CodeEditor::Edited, d);
            d->mergeType = Diff::KeepLocal;
            break;

        case Diff::SegmentType::DifferentOnBoth:
            if (isEmpty(d->local)) {
                m_ui.plainTextEditMine->append(d->local, CodeEditor::Edited, d);
                m_ui.plainTextEditTheir->append(d->remote, CodeEditor::Added, d);
                //                d->mergeType = Diff::KeepRemote;
            } else if (isEmpty(d->remote)) {
                m_ui.plainTextEditMine->append(d->local, CodeEditor::Added, d);
                m_ui.plainTextEditTheir->append(d->remote, CodeEditor::Edited, d);
                //                d->mergeType = Diff::KeepLocal;
            } else {
                m_ui.plainTextEditMine->append(d->local, CodeEditor::Edited, d);
                m_ui.plainTextEditTheir->append(d->remote, CodeEditor::Edited, d);
            }
            m_ui.plainTextEditBase->append(d->base, CodeEditor::Edited, d);
            d->mergeType = Diff::None;
            break;
        }
    }
    updateResult();

    QFileInfo fi;
    if (_filePathResult.isEmpty())
        fi.setFile(_filePathBase);
    else
        fi.setFile(_filePathResult);

    setWindowTitle(fi.fileName() + "[*]");
    setWindowModified(true);
}

void GitKlientMergeWindow::updateResult()
{
    m_ui.plainTextEditResult->clearAll();
    for (auto &d : _diffs) {
        if (d->type == Diff::SegmentType::SameOnBoth) {
            m_ui.plainTextEditResult->append(d->base, CodeEditor::Unchanged, d);
            continue;
        }
        switch (d->mergeType) {
        case Diff::None: {
            switch (d->type) {
            case Diff::SegmentType::SameOnBoth:
                m_ui.plainTextEditResult->append(d->base, CodeEditor::Unchanged, d);
                break;

            case Diff::SegmentType::OnlyOnRight:
                m_ui.plainTextEditResult->append(d->remote, CodeEditor::Added, d);
                break;

            case Diff::SegmentType::OnlyOnLeft:
                m_ui.plainTextEditResult->append(d->local, CodeEditor::Added, d);
                break;

            case Diff::SegmentType::DifferentOnBoth:
                if (d->local == d->remote)
                    m_ui.plainTextEditResult->append(d->remote, CodeEditor::Added, d); // Not changed
                else if (isEmpty(d->local))
                    m_ui.plainTextEditResult->append(d->remote, CodeEditor::Added, d);
                else if (isEmpty(d->remote))
                    m_ui.plainTextEditResult->append(d->local, CodeEditor::Added, d);
                else
                    m_ui.plainTextEditResult->append(" ", CodeEditor::Removed, d);
                break;
            }
            break;
        }

        case Diff::KeepLocal:
            m_ui.plainTextEditResult->append(d->local, CodeEditor::Edited, d);
            break;

        case Diff::KeepRemote:
            m_ui.plainTextEditResult->append(d->remote, CodeEditor::Edited, d);
            break;

        case Diff::KeepLocalThenRemote:
            m_ui.plainTextEditResult->append(d->local, CodeEditor::Edited, d);
            m_ui.plainTextEditResult->append(d->remote, CodeEditor::Edited, d);
            break;

        case Diff::KeepRemoteThenLocal:
            m_ui.plainTextEditResult->append(d->remote, CodeEditor::Edited, d);
            m_ui.plainTextEditResult->append(d->local, CodeEditor::Edited, d);
            break;

        default:
            m_ui.plainTextEditResult->append("***", CodeEditor::Edited, d);
            break;
        }
    }
    _conflictsLabel->setText(i18n("Conflicts: %1", _mapper->conflicts()));
}

void GitKlientMergeWindow::initActions()
{
    KActionCollection *actionCollection = this->actionCollection();

    auto actionKeepMine = actionCollection->addAction("keep_mine",
                                                      this,
                                                      &GitKlientMergeWindow::actionKeepMine_clicked);
    actionKeepMine->setText(i18n("Keep mine"));
    actionKeepMine->setIcon(QIcon::fromTheme("diff-keep-mine"));
    actionCollection->setDefaultShortcut(actionKeepMine, QKeySequence("Ctrl+L"));

    auto actionKeepTheir = actionCollection
                               ->addAction("keep_their",
                                           this,
                                           &GitKlientMergeWindow::actionKeepTheir_clicked);
    actionKeepTheir->setText(i18n("Keep their"));
    actionKeepTheir->setIcon(QIcon::fromTheme("diff-keep-their"));
    actionCollection->setDefaultShortcut(actionKeepTheir, QKeySequence("Ctrl+R"));

    auto actionKeepMineBeforeTheir
        = actionCollection->addAction("keep_mine_before_their",
                                      this,
                                      &GitKlientMergeWindow::actionKeepMineBeforeTheir_clicked);

    actionKeepMineBeforeTheir->setText(i18n("Keep mine before their"));
    actionKeepMineBeforeTheir->setIcon(QIcon::fromTheme("diff-keep-mine-before-their"));
    actionCollection->setDefaultShortcut(actionKeepMineBeforeTheir, QKeySequence("Ctrl+Shift+L"));

    auto actionKeepTheirBeforeMine
        = actionCollection->addAction("keep_their_before_mine",
                                      this,
                                      &GitKlientMergeWindow::actionKeepTheirBeforeMine_clicked);
    actionKeepTheirBeforeMine->setText(i18n("Keep their before mine"));
    actionKeepTheirBeforeMine->setIcon(QIcon::fromTheme("diff-keep-their-before-mine"));
    actionCollection->setDefaultShortcut(actionKeepTheirBeforeMine, QKeySequence("Ctrl+Shift+R"));

    auto actionKeepMyFile = actionCollection
                                ->addAction("keep_my_file",
                                            this,
                                            &GitKlientMergeWindow::actionKeepMyFile_clicked);
    actionKeepMyFile->setText(i18n("Keep my file"));
    actionKeepMyFile->setIcon(QIcon::fromTheme("diff-keep-mine-file"));
    actionCollection->setDefaultShortcut(actionKeepMyFile, QKeySequence("Ctrl+Alt+L"));

    auto actionKeepTheirFile = actionCollection
                                   ->addAction("keep_their_file",
                                               this,
                                               &GitKlientMergeWindow::actionKeepTheirFile_clicked);
    actionKeepTheirFile->setText(i18n("Keep their file"));
    actionKeepTheirFile->setIcon(QIcon::fromTheme("diff-keep-their-file"));
    actionCollection->setDefaultShortcut(actionKeepTheirFile, QKeySequence("Ctrl+Alt+R"));

    /*auto actionGotoPrevDiff = actionCollection
                                  ->addAction("goto_prev_diff",
                                              this,
                                              &GitKlientMergeWindow::actionGotoPrevDiff_clicked);
    actionGotoPrevDiff->setText(i18n("Previous diff"));
    actionGotoPrevDiff->setIcon(QIcon::fromTheme("diff-goto-prev-diff"));
    actionCollection->setDefaultShortcut(actionGotoPrevDiff, QKeySequence(Qt::Key_PageUp));

    auto actionGotoNextDiff = actionCollection
                                  ->addAction("goto_next_diff",
                                              this,
                                              &GitKlientMergeWindow::actionGotoNextDiff_clicked);
    actionGotoNextDiff->setText(i18n("Next diff"));
    actionGotoNextDiff->setIcon(QIcon::fromTheme("diff-goto-next-diff"));
    actionCollection->setDefaultShortcut(actionGotoNextDiff, QKeySequence(Qt::Key_PageDown));
*/
    KStandardAction::open(this, &GitKlientMergeWindow::fileOpen, actionCollection);
    KStandardAction::save(this, &GitKlientMergeWindow::fileSave, actionCollection);
    KStandardAction::quit(qApp, &QApplication::closeAllWindows, actionCollection);
    KStandardAction::preferences(this, &GitKlientMergeWindow::preferences, actionCollection);

    _codeEditorContextMenu = new QMenu(this);
    _codeEditorContextMenu->addActions({actionKeepMine, actionKeepTheir});
    _codeEditorContextMenu->addSeparator();
    _codeEditorContextMenu->addActions({actionKeepMineBeforeTheir, actionKeepTheirBeforeMine});
    _codeEditorContextMenu->addSeparator();
    _codeEditorContextMenu->addActions({actionKeepMyFile, actionKeepTheirFile});
}

void GitKlientMergeWindow::doMergeAction(Diff::MergeType type)
{
    auto s = _mapper->currentSegment();

    if (!s)
        return;

    if (s->type == Diff::SegmentType::SameOnBoth)
        return;

    auto ss = static_cast<Diff::MergeSegment *>(s);
    ss->mergeType = type;
    updateResult();
//    m_ui.plainTextEditResult->highlightSegment(s);

    _mapper->setCurrentSegment(s);
}

bool GitKlientMergeWindow::isFullyResolved()
{
    for (auto &d : _diffs)
        if (d->mergeType == Diff::None && d->type == Diff::SegmentType::DifferentOnBoth)
            return false;
    return true;
}

void GitKlientMergeWindow::closeEvent(QCloseEvent *event)
{
    if (isWindowModified()) {
        MergeCloseEventDialog d(this);
        auto r = d.exec();

        switch (r) {
        case MergeCloseEventDialog::MarkAsResolved:
            fileSave();
            accept();
            break;
        case MergeCloseEventDialog::LeaveAsIs:
            reject();
            break;
        case MergeCloseEventDialog::DontExit:
            event->ignore();
            break;
        }
    }
    return;;
    if (isWindowModified()) {
        auto btn = KMessageBox::questionYesNo(this,
                                              i18n("Changes is not saved, exit?"),
                                              i18n("Close"));

        if (btn == KMessageBox::Yes)
            event->accept();
        else
            event->ignore();
        return;
    }
    if (isFullyResolved()) {
        event->accept();
    } else {
        auto btn = KMessageBox::questionYesNo(this,
                                              i18n("The comflicts are not fully resolved, exit?"),
                                              i18n("Close"));

        if (btn == KMessageBox::Yes)
            event->accept();
        else
            event->ignore();
    }
}

const QString &GitKlientMergeWindow::filePathResult() const
{
    return _filePathResult;
}

void GitKlientMergeWindow::setFilePathResult(const QString &newFilePathResult)
{
    _filePathResult = newFilePathResult;
}

void GitKlientMergeWindow::fileSave()
{
    QFile f(_filePathResult);
    if (!f.open(QIODevice::Text | QIODevice::WriteOnly)) {
        KMessageBox::information(this, i18n("Unable to open the file") + _filePathResult);
        return;
    }
    f.write(m_ui.plainTextEditResult->toPlainText().toUtf8());
    f.close();
    setWindowModified(false);
}

void GitKlientMergeWindow::fileOpen()
{
    MergeOpenFilesDialog d;
    if (d.exec() == QDialog::Accepted) {
        setFilePathBase(d.filePathBase());
        setFilePathLocal(d.filePathLocal());
        setFilePathRemote(d.filePathRemote());
        load();
    }
}

void GitKlientMergeWindow::preferences()
{
    SettingsManager::instance()->exec(this);
}

void GitKlientMergeWindow::actionKeepMine_clicked()
{
    doMergeAction(Diff::MergeType::KeepLocal);
}

void GitKlientMergeWindow::actionKeepTheir_clicked()
{
    doMergeAction(Diff::MergeType::KeepRemote);
}

void GitKlientMergeWindow::actionKeepMineBeforeTheir_clicked()
{
    doMergeAction(Diff::MergeType::KeepLocalThenRemote);
}

void GitKlientMergeWindow::actionKeepTheirBeforeMine_clicked()
{
    doMergeAction(Diff::MergeType::KeepRemoteThenLocal);
}

void GitKlientMergeWindow::actionKeepMyFile_clicked()
{
    m_ui.plainTextEditResult->setPlainText(m_ui.plainTextEditMine->toPlainText());
}

void GitKlientMergeWindow::actionKeepTheirFile_clicked()
{
    m_ui.plainTextEditResult->setPlainText(m_ui.plainTextEditTheir->toPlainText());
}

void GitKlientMergeWindow::actionGotoPrevDiff_clicked()
{

}

void GitKlientMergeWindow::actionGotoNextDiff_clicked()
{

}

void GitKlientMergeWindow::codeEditors_customContextMenuRequested(QPoint pos)
{
    Q_UNUSED(pos)
    _codeEditorContextMenu->popup(QCursor::pos());
}

const QString &GitKlientMergeWindow::filePathBase() const
{
    return _filePathBase;
}

void GitKlientMergeWindow::setFilePathBase(const QString &newFilePathBase)
{
    _filePathBase = newFilePathBase;
}

const QString &GitKlientMergeWindow::filePathRemote() const
{
    return _filePathRemote;
}

void GitKlientMergeWindow::setFilePathRemote(const QString &newFilePathRemote)
{
    _filePathRemote = newFilePathRemote;
}

const QString &GitKlientMergeWindow::filePathLocal() const
{
    return _filePathLocal;
}

void GitKlientMergeWindow::setFilePathLocal(const QString &newFilePathLocal)
{
    _filePathLocal = newFilePathLocal;
}

void GitKlientMergeWindow::on_plainTextEditResult_textChanged()
{
//    auto segment = static_cast<Diff::MergeSegment *>(_mapper->currentSegment());
//    if (segment) {
//        segment->mergeType = Diff::MergeCustom;
//    }
}

