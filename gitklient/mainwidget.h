#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "ui_mainwidget.h"

namespace Git {
class Manager;
}
class TreeModel;
class ButtonsGroup;
class MainWidget : public QWidget, private Ui::MainWidget
{
    Q_OBJECT
    int _listIndex{-1};
    QStringList _branches;
    Git::Manager *_git{nullptr};
    TreeModel *_repoModel;
    ButtonsGroup *_pagesButtonsGroup;
    QMenu *_branchesMenu;

public:
    explicit MainWidget(QWidget *parent = nullptr);
    virtual ~MainWidget();

    Git::Manager *git() const;
    void setGit(Git::Manager *newGit);

private slots:
    void reload();
    void listButton_clicked();
    void pageButtons_clicked(int index);

    void on_treeViewRepo_activated(const QModelIndex &index);
    void on_treeViewRepo_customContextMenuRequested(const QPoint &pos);

    void on_actionBrowseBranch_triggered();
    void on_actionDiffBranch_triggered();
    void on_pushButtonAddTag_clicked();
};

#endif // MAINWIDGET_H
