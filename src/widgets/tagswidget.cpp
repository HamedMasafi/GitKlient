#include "tagswidget.h"

#include "git/gitmanager.h"
#include "dialogs/taginfodialog.h"

TagsWidget::TagsWidget(QWidget *parent) :
      WidgetBase(parent)
{
    setupUi(this);
}

TagsWidget::TagsWidget(Git::Manager *git, GitKlientWindow *parent):
      WidgetBase(git, parent)
{
    setupUi(this);
}

void TagsWidget::reload()
{
    listWidgetTags->clear();
    listWidgetTags->addItems(git()->tags());
}

void TagsWidget::on_pushButtonAddTag_clicked()
{
    TagInfoDialog d(this);
    d.setWindowTitle(i18n("New tag"));
    if (d.exec()==QDialog::Accepted) {
        git()->createTag(d.tagName(), d.message());
    }
}

