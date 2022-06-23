#ifndef BRANCHESSTATUSWIDGET_H
#define BRANCHESSTATUSWIDGET_H

#include "ui_branchesstatuswidget.h"
#include "widgetbase.h"

namespace Git {
class Manager;
class BranchesModel;
}
class BranchActions;
class BranchesStatusWidget : public WidgetBase, private Ui::BranchesStatusWidget
{
    Q_OBJECT

    QStringList _branches;
    BranchActions *_actions;
    Git::BranchesModel *_model;

public:
    explicit BranchesStatusWidget(QWidget *parent = nullptr);
    explicit BranchesStatusWidget(Git::Manager *git, AppWindow *parent = nullptr);

    void saveState(QSettings &settings) const override;
    void restoreState(QSettings &settings) override;

    void init(Git::Manager *git);

private slots:
    void on_comboBoxReferenceBranch_currentIndexChanged(const QString &selectedBranch);
    void on_pushButtonRemoveSelected_clicked();
    void on_treeView_customContextMenuRequested(const QPoint &pos);

    void reload() override;
};

#endif // BRANCHESSTATUSWIDGET_H
