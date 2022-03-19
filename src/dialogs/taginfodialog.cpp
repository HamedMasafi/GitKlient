#include "taginfodialog.h"

TagInfoDialog::TagInfoDialog(QWidget *parent) :
      Dialog(parent)
{
    setupUi(this);
}

QString TagInfoDialog::tagName() const
{
    return lineEditTagName->text();
}

void TagInfoDialog::setTagName(const QString &newTagName)
{
    lineEditTagName->setText(newTagName);
}

QString TagInfoDialog::message() const
{
    return lineEditMessage->text();
}

void TagInfoDialog::setMessage(const QString &newMessage)
{
    lineEditMessage->setText(newMessage);
}
