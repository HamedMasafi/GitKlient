// Copyright (C) 2020 Hamed Masafi <hamed.masafi@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

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
    void init();
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
