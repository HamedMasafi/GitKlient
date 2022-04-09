#include "tagswidget.h"

#include "git/gitmanager.h"
#include "git/models/tagsmodel.h"

#include "dialogs/taginfodialog.h"
#include "actions/tagsactions.h"

TagsWidget::TagsWidget(QWidget *parent) :
      WidgetBase(parent)
{
    setupUi(this);
    treeViewTags->setModel(Git::Manager::instance()->tagsModel());

    _actions = new TagsActions(Git::Manager::instance(), this);
    pushButtonAddTag->setAction(_actions->actionCreate());
    pushButtonRemove->setAction(_actions->actionRemove());
    pushButtonCheckout->setAction(_actions->actionCheckout());
}

TagsWidget::TagsWidget(Git::Manager *git, GitKlientWindow *parent):
      WidgetBase(git, parent)
{
    setupUi(this);
    treeViewTags->setModel(git->tagsModel());
    _actions = new TagsActions(git, this);
    pushButtonAddTag->setAction(_actions->actionCreate());
    pushButtonRemove->setAction(_actions->actionRemove());
    pushButtonCheckout->setAction(_actions->actionCheckout());
}

void TagsWidget::reload()
{
//    listWidgetTags->clear();
//    listWidgetTags->addItems(git()->tags());
}

void TagsWidget::on_pushButtonAddTag_clicked()
{
    TagInfoDialog d(this);
    d.setWindowTitle(i18n("New tag"));
    if (d.exec() == QDialog::Accepted) {
        git()->createTag(d.tagName(), d.message());
    }
}

void TagsWidget::saveState(QSettings &settings) const
{
    save(settings, treeViewTags);
}

void TagsWidget::restoreState(QSettings &settings)
{
    restore(settings, treeViewTags);
}

void TagsWidget::on_treeViewTags_customContextMenuRequested(const QPoint &pos)
{
    _actions->popup();
}

