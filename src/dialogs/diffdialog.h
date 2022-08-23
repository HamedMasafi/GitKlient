#ifndef DIFFDIALOG_H
#define DIFFDIALOG_H

#include "../core/appdialog.h"
#include "ui_diffdialog.h"
#include <git/gitfile.h>

class DiffTreeModel;
class DiffDialog : public AppDialog, private Ui::DiffDialog
{
    Q_OBJECT
    Git::File _oldFile;
    Git::File _newFile;

    QString _oldBranch;
    QString _newBranch;

    DiffTreeModel *_diffModel{};

public:
    Q_DECL_DEPRECATED explicit DiffDialog(QWidget *parent = nullptr);
    Q_DECL_DEPRECATED DiffDialog(const Git::File &oldFile, const Git::File &newFile, QWidget *parent = nullptr);
    Q_DECL_DEPRECATED DiffDialog(const QString &oldBranch, const QString &newBranch, QWidget *parent = nullptr);

private slots:
    void on_toolButtonShowHiddenChars_clicked(bool checked);

    void on_pushButtonSaveAs_clicked();

    void on_treeView_clicked(const QModelIndex &index);

private:
    void compare(const QStringList &oldData, const QStringList &newData);
};

#endif // DIFFDIALOG_H
