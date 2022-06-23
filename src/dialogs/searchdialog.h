#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include "../core/appdialog.h"
#include "ui_searchdialog.h"

namespace Git {
class Manager;
}

class QStandardItemModel;
class SearchDialog : public AppDialog, private Ui::SearchDialog
{
    Q_OBJECT
    Git::Manager *_git;

    struct {
        int value;
        int total;
        QString message;
    } _progress;
    QStandardItemModel *_model{nullptr};

public:
    explicit SearchDialog(const QString &path, Git::Manager *git, QWidget *parent = nullptr);
    explicit SearchDialog(Git::Manager *git, QWidget *parent = nullptr);

    void initModel();

public slots:
    void on_pushButtonSearch_clicked();
    void on_treeView_doubleClicked(QModelIndex index);

private:
    void beginSearch();
    void searchOnPlace(const QString &place, const QString &commit);

    // QObject interface
protected:
    void timerEvent(QTimerEvent *event) override;
};

#endif // SEARCHDIALOG_H
