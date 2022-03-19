#ifndef SUBMODULEINFODIALOG_H
#define SUBMODULEINFODIALOG_H

#include "../core/dialog.h"
#include "ui_submoduleinfodialog.h"

class SubmoduleInfoDialog : public Dialog, private Ui::SubmoduleInfoDialog
{
    Q_OBJECT

public:
    explicit SubmoduleInfoDialog(QWidget *parent = nullptr);

    bool force() const;
    void setForce(bool newForce);

    QString url() const;
    void setUrl(const QString &newUrl);

    QString path() const;
    void setPath(const QString &newPath);

    QString branch() const;
    void setBranch(const QString &newBranch);
};

#endif // SUBMODULEINFODIALOG_H
