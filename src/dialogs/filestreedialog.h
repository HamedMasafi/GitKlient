#ifndef FILESTREEDIALOG_H
#define FILESTREEDIALOG_H

#include "../core/appdialog.h"
#include "ui_filestreedialog.h"

class FileActions;
class TreeModel;
class FilesTreeDialog : public AppDialog, private Ui::FilesTreeDialog
{
    Q_OBJECT
    TreeModel *_treeModel;
    QString _place;
    FileActions *_actions;

public:
    explicit FilesTreeDialog(const QString &place, QWidget *parent = nullptr);

private Q_SLOTS:
    void on_treeView_clicked(const QModelIndex &index);
    void on_listWidget_customContextMenuRequested(const QPoint &pos);

};

#endif // FILESTREEDIALOG_H
