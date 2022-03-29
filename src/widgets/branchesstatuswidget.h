#ifndef BRANCHESSTATUSWIDGET_H
#define BRANCHESSTATUSWIDGET_H

#include "ui_branchesstatuswidget.h"
#include "widgetbase.h"

namespace Git {
class Manager;
class BranchesCache;
}
class BranchActions;
class BranchesStatusWidget : public WidgetBase, private Ui::BranchesStatusWidget
{
    Q_OBJECT
    QStringList _branches;
    BranchActions *_actions;
    Git::BranchesCache *_model;

public:
    explicit BranchesStatusWidget(QWidget *parent = nullptr);
    explicit BranchesStatusWidget(Git::Manager *git, GitKlientWindow *parent = nullptr);

    void saveState(QSettings &settings) const override;
    void restoreState(QSettings &settings) override;

    void initGit(Git::Manager *git);

private slots:
    void on_comboBoxReferenceBranch_currentIndexChanged(const QString &selectedBranch);
    void on_pushButtonRemoveSelected_clicked();
    void on_treeView_customContextMenuRequested(const QPoint &pos);

    void reload() override;
    void on_pushButtonBrowse_clicked();
    void on_pushButtonDiff_clicked();
    void on_pushButtonCheckout_clicked();
    void on_pushButtonNew_clicked();
};

#endif // BRANCHESSTATUSWIDGET_H
