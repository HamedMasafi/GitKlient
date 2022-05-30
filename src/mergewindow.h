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

#ifndef GitKlientMergeWindow_H
#define GitKlientMergeWindow_H

#include "GitKlientSettings.h"
#include "core/appmainwindow.h"
#include "diff/diff.h"
#include "ui_gitklientmergeview.h"

class GitKlientView;
class SegmentsMapper;
class QLabel;
class MergeWindow : public AppMainWindow
{
    Q_OBJECT
public:
    enum Mode
    {
        NoParams,
        MergeByParams
    };

    explicit MergeWindow(Mode mode = NoParams, QWidget *parent = nullptr);
    ~MergeWindow() override;

    void load();

    const QString &filePathLocal() const;
    void setFilePathLocal(const QString &newFilePathLocal);

    const QString &filePathRemote() const;
    void setFilePathRemote(const QString &newFilePathRemote);

    const QString &filePathBase() const;
    void setFilePathBase(const QString &newFilePathBase);

    const QString &filePathResult() const;
    void setFilePathResult(const QString &newFilePathResult);

private Q_SLOTS:
    void fileSave();
    void fileOpen();
    void preferences();

    void actionKeepMine_clicked();
    void actionKeepTheir_clicked();
    void actionKeepMineBeforeTheir_clicked();
    void actionKeepTheirBeforeMine_clicked();
    void actionKeepMyFile_clicked();
    void actionKeepTheirFile_clicked();
    void actionGotoPrevDiff_clicked();
    void actionGotoNextDiff_clicked();

    void actionViewFiles_clicked();
    void actionViewBlocks_clicked();

    void codeEditors_customContextMenuRequested(QPoint pos);

    void on_plainTextEditResult_textChanged();
    void on_plainTextEditResult_blockSelected();

private:
    Ui::Form m_ui;
    void updateResult();
    void initActions();
    void doMergeAction(Diff::MergeType type);
    bool isFullyResolved();

    bool _scrollLockMine{false};
    bool _scrollLockTheir{false};
    QList<Diff::MergeSegment *> _diffs;
    QMenu *_codeEditorContextMenu;
    SegmentsMapper *_mapper;

    QString _filePathLocal;
    QString _filePathRemote;
    QString _filePathBase;
    QString _filePathResult;
    Mode _mode;
    QLabel *_conflictsLabel;
    QAction *_actionBlocksView;
    QAction *_actionFilesView;

protected:
    void closeEvent(QCloseEvent *event) override;
};

#endif // GitKlientMergeWindow_H
