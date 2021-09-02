#ifndef TAGINFODIALOG_H
#define TAGINFODIALOG_H

#include "ui_taginfodialog.h"

class TagInfoDialog : public QDialog, private Ui::TagInfoDialog
{
    Q_OBJECT

public:
    explicit TagInfoDialog(QWidget *parent = nullptr);
    QString tagName() const;
    void setTagName(const QString &newTagName);
    QString message() const;
    void setMessage(const QString &newMessage);
};

#endif // TAGINFODIALOG_H
