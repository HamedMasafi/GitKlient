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

#include "closeeventdialog.h"
#include "gitklientmergewindow.h"

#include "GitKlientMergeSettings.h"
#include "openfilesdialog.h"
#include "diff/segmentsmapper.h"
#include "widgets/editactionsmapper.h"

#include <KMessageBox>
#include <QDebug>
#include <QFile>
#include <QScrollBar>
#include <QTextBlock>

#include <KActionCollection>
#include <KConfigDialog>
#include <KMessageBox>
#include <QFileInfo>
#include <QMenu>


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
    QFile f{filePath};
    if (!f.open(QIODevice::ReadOnly))
        return QStringList();
    auto buf = QString(f.readAll()).split('\n');
    f.close();
    return buf;
}

void GitKlientMergeWindow::load()
{
    auto baseList = readFile(_filePathBase);
    auto localList = readFile(_filePathLocal);
    auto remoteList = readFile(_filePathRemote);

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
            d->mergeType = Diff::KeepLocal;
            break;
        }

        case Diff::SegmentType::OnlyOnRight:
            m_ui.plainTextEditMine->append(d->local, CodeEditor::Removed, d);
            m_ui.plainTextEditTheir->append(d->remote, CodeEditor::Added, d);
            d->mergeType = Diff::KeepRemote;
            break;
        case Diff::SegmentType::OnlyOnLeft:
            m_ui.plainTextEditMine->append(d->local, CodeEditor::Added, d);
            m_ui.plainTextEditTheir->append(d->remote, CodeEditor::Removed, d);
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

GitKlientMergeWindow::GitKlientMergeWindow(Mode mode, QWidget *parent) : KXmlGuiWindow(parent)
{
    initActions();
    auto mapper = new EditActionsMapper;
    mapper->init(actionCollection());
    setupGUI();
    auto w = new QWidget(this);
    m_ui.setupUi(w);
    setCentralWidget(w);


    mapper->addTextEdit(m_ui.plainTextEditMine);
    mapper->addTextEdit(m_ui.plainTextEditTheir);
    mapper->addTextEdit(m_ui.plainTextEditResult);


    _mapper = new SegmentsMapper;

#ifdef QT_DEBUG
    _filePathBase = "/doc/dev/qt/git-diff/samples/sample_BASE.h";
    _filePathLocal = "/doc/dev/qt/git-diff/samples/sample_LOCAL.h";
    _filePathRemote = "/doc/dev/qt/git-diff/samples/sample_REMOTE.h";
    _filePathResult = "/doc/dev/qt/git-diff/samples/sample.h";
    load();
#endif

    connect(m_ui.plainTextEditMine,
            &CodeEditor::blockSelected,
            this,
            &GitKlientMergeWindow::plainTextEditMine_blockSelected);

    connect(m_ui.plainTextEditTheir,
            &CodeEditor::blockSelected,
            this,
            &GitKlientMergeWindow::plainTextEditTheir_blockSelected);

    connect(m_ui.plainTextEditMine->verticalScrollBar(),
            &QScrollBar::valueChanged,
            this,
            &GitKlientMergeWindow::plainTextEditMine_scroll);

    connect(m_ui.plainTextEditTheir->verticalScrollBar(),
            &QScrollBar::valueChanged,
            this,
            &GitKlientMergeWindow::plainTextEditTheir_scroll);

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

}

GitKlientMergeWindow::~GitKlientMergeWindow() {}

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
                if (isEmpty(d->local))
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

    auto actionGotoPrevDiff = actionCollection
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

    KStandardAction::open(this, &GitKlientMergeWindow::fileOpen, actionCollection);
    KStandardAction::save(this, &GitKlientMergeWindow::fileSave, actionCollection);
    KStandardAction::quit(qApp, &QApplication::closeAllWindows, actionCollection);
    KStandardAction::preferences(this, &GitKlientMergeWindow::settingsConfigure, actionCollection);

    _codeEditorContextMenu = new QMenu(this);
    _codeEditorContextMenu->addActions({actionKeepMine, actionKeepTheir});
    _codeEditorContextMenu->addSeparator();
    _codeEditorContextMenu->addActions({actionKeepMineBeforeTheir, actionKeepTheirBeforeMine});
    _codeEditorContextMenu->addSeparator();
    _codeEditorContextMenu->addActions({actionKeepMyFile, actionKeepTheirFile});
}

void GitKlientMergeWindow::doMergeAction(Diff::MergeType type)
{
    if (!m_ui.plainTextEditMine->hasFocus() && !m_ui.plainTextEditTheir->hasFocus())
        return;

    CodeEditor *from;
    CodeEditor *to;
    if (m_ui.plainTextEditMine->hasFocus()) {
        from = m_ui.plainTextEditMine;
        to = from = m_ui.plainTextEditTheir;
    } else {
        from = from = m_ui.plainTextEditTheir;
        to = m_ui.plainTextEditMine;
    }
    auto s = from->currentSegment();

    if (!s)
        return;

    if (s->type == Diff::SegmentType::SameOnBoth)
        return;
    if (s) {
        auto ss = static_cast<Diff::MergeSegment *>(s);
        ss->mergeType = type;
        updateResult();
        m_ui.plainTextEditResult->highlightSegment(s);
    }
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
        CloseEventDialog d(this);
        auto r = d.exec();

        switch (r) {
        case CloseEventDialog::MarkAsResolved:
            fileSave();
            break;
        case CloseEventDialog::LeaveAsIs:
            break;
        case CloseEventDialog::DontExit:
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
    OpenFilesDialog d;
    if (d.exec() == QDialog::Accepted) {
        setFilePathBase(d.filePathBase());
        setFilePathLocal(d.filePathLocal());
        setFilePathRemote(d.filePathRemote());
        load();
    }
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
    _codeEditorContextMenu->popup(qobject_cast<QWidget*>(sender())->mapToGlobal(pos));
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

void GitKlientMergeWindow::plainTextEditMine_scroll(int value)
{
    static bool b{false};
    if (b)
        return;
    b = true;
    m_ui.plainTextEditTheir->verticalScrollBar()->setValue(
        (int) (((float) value / (float) m_ui.plainTextEditTheir->verticalScrollBar()->maximum())
               * (float) m_ui.plainTextEditTheir->verticalScrollBar()->maximum()));
    b = false;
    m_ui.widgetSegmentsConnector->update();
}

void GitKlientMergeWindow::plainTextEditTheir_scroll(int value)
{
    static bool b{false};
    if (b)
        return;
    b = true;
    m_ui.plainTextEditMine->verticalScrollBar()->setValue(
        (int) (((float) value / (float) m_ui.plainTextEditMine->verticalScrollBar()->maximum())
               * (float) m_ui.plainTextEditMine->verticalScrollBar()->maximum()));
    b = false;
    m_ui.widgetSegmentsConnector->update();
}

void GitKlientMergeWindow::plainTextEditMine_blockSelected()
{
    auto l = m_ui.plainTextEditMine->currentLineNumber();
    auto n = _mapper->mapIndexFromOldToNew(l);
    if (n != -1)
        m_ui.plainTextEditTheir->gotoLineNumber(n);
    return;

    //    auto b = plainTextEditMine->textCursor().block().blockNumber();
    auto b = m_ui.plainTextEditMine->currentSegment();
    if (b) {
        m_ui.widgetSegmentsConnector->setCurrentSegment(b);
        qDebug() << "Block found" << b->oldText;
        m_ui.plainTextEditTheir->highlightSegment(b);
    }
}

void GitKlientMergeWindow::plainTextEditTheir_blockSelected()
{
    auto l = m_ui.plainTextEditTheir->currentLineNumber();
    auto n = _mapper->map(2, 1, l);
    if (n != -1)
        m_ui.plainTextEditMine->gotoLineNumber(n);

    //    auto b = m_ui.plainTextEditTheir->currentSegment();
    //    if (b) {
    //        m_ui.widgetSegmentsConnector->setCurrentSegment(b);
    //        m_ui.plainTextEditMine->highlightSegment(b);
    //    }
}

void GitKlientMergeWindow::on_plainTextEditResult_textChanged()
{
    //    auto segment = m_ui.plainTextEditResult->currentSegment();
    //    if (segment) {
    //        segment->mergeType = Diff::MergeCustom;
    //    }
}

void GitKlientMergeWindow::settingsConfigure()
{
//    qCDebug(GITKLIENTDIFF) << "GitKlientDiffWindow::settingsConfigure()";
    // The preference dialog is derived from prefs_base.ui
    //
    // compare the names of the widgets in the .ui file
    // to the names of the variables in the .kcfg file
    //avoid to have 2 dialogs shown
    if (KConfigDialog::showDialog(QStringLiteral("settings"))) {
        return;
    }

    KConfigDialog *dialog = new KConfigDialog(this, QStringLiteral("settings"), GitKlientMergeSettings::self());
    QWidget *generalSettingsPage = new QWidget;
//    settingsBase.setupUi(generalSettingsPage);
    dialog->addPage(generalSettingsPage, i18n("General"), QStringLiteral("package_setting"));
    //    connect(dialog, &KConfigDialog::settingsChanged, m_gitklientdiffView, &GitKlientDiffView::handleSettingsChanged);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->show();
}
