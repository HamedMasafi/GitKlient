#ifndef FILESTREEDIALOG_H
#define FILESTREEDIALOG_H

#include "ui_filestreedialog.h"

class TreeModel;
class FilesTreeDialog : public QDialog, private Ui::FilesTreeDialog
{
    Q_OBJECT
    TreeModel *_treeModel;
    QString _place;
    QMenu *_fileMenu;

public:
    explicit FilesTreeDialog(const QString &place, QWidget *parent = nullptr);

public Q_SLOTS:
    void on_treeView_clicked(const QModelIndex &index);
private slots:
    void viewFile();
    void copyFile();
    void logFile();
    void blameFile();
    void search();
    void on_listWidget_customContextMenuRequested(const QPoint &pos);

};

#endif // FILESTREEDIALOG_H
