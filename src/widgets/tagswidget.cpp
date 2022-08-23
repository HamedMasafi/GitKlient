#include "tagswidget.h"

#include "git/gittag.h"
#include "git/gitmanager.h"
#include "git/models/tagsmodel.h"

#include "dialogs/taginfodialog.h"
#include "actions/tagsactions.h"

TagsWidget::TagsWidget(QWidget *parent) :
      WidgetBase(parent)
{
    setupUi(this);
    _model = Git::Manager::instance()->tagsModel();
    treeViewTags->setModel(_model);
    _actions = new TagsActions(Git::Manager::instance(), this);
    pushButtonAddTag->setAction(_actions->actionCreate());
    pushButtonRemove->setAction(_actions->actionRemove());
    pushButtonCheckout->setAction(_actions->actionCheckout());
    pushButtonPush->setAction(_actions->actionPush());
}

TagsWidget::TagsWidget(Git::Manager *git, AppWindow *parent):
      WidgetBase(git, parent)
{
    setupUi(this);
    _model = git->tagsModel();
    treeViewTags->setModel(_model);
    _actions = new TagsActions(git, this);
    pushButtonAddTag->setAction(_actions->actionCreate());
    pushButtonRemove->setAction(_actions->actionRemove());
    pushButtonCheckout->setAction(_actions->actionCheckout());
    pushButtonPush->setAction(_actions->actionPush());
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
    Q_UNUSED(pos)
    auto item = _model->fromIndex(treeViewTags->currentIndex());
    if (item) {
        _actions->setTagName(item->name());
        _actions->popup();
    }
}

void TagsWidget::on_treeViewTags_itemActivated(const QModelIndex &index)
{
    auto item = _model->fromIndex(index);
    if (item)
        _actions->setTagName(item->name());
}

