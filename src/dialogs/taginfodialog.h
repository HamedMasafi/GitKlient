#ifndef TAGINFODIALOG_H
#define TAGINFODIALOG_H

#include "../core/appdialog.h"
#include "ui_taginfodialog.h"

class TagInfoDialog : public AppDialog, private Ui::TagInfoDialog
{
    Q_OBJECT

public:
    explicit TagInfoDialog(QWidget *parent = nullptr);
    QString tagName() const;
    void setTagName(const QString &newTagName);
    QString message() const;
    void setMessage(const QString &newMessage);

private slots:
    void on_lineEditTagName_textChanged(QString s);
};

#endif // TAGINFODIALOG_H
