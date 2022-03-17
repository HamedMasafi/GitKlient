#ifndef MERGEDIALOG_H
#define MERGEDIALOG_H

#include "ui_mergedialog.h"

class MergeDialog : public QDialog, private Ui::MergeDialog
{
    Q_OBJECT
public:
    explicit MergeDialog(QWidget *parent = nullptr);
    explicit MergeDialog(const QString &sourceBranch, QWidget *parent = nullptr);

private:
    QString _sourceBranch;
};

#endif // MERGEDIALOG_H
