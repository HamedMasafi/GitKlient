#include "stasheswidget.h"
#include "git/gitmanager.h"

#include <QInputDialog>

StashesWidget::StashesWidget(QWidget *parent) :
      WidgetBase(parent)
{
    setupUi(this);
}

StashesWidget::StashesWidget(Git::Manager *git, QWidget *parent) : WidgetBase(git, parent)
{
    setupUi(this);
}

void StashesWidget::reload()
{
    listWidget->clear();
    listWidget->addItems(git()->stashes());
}

void StashesWidget::on_pushButtonCreateNew_clicked()
{
    bool ok;
    QInputDialog d;
    auto name = d.getText(this,
                          i18n("Create new stash"),
                          i18n("Name of stash"),
                          QLineEdit::Normal,
                          QString(),
                          &ok);

    if (ok) {
        git()->createStash(name);
        reload();
    }
}


void StashesWidget::on_pushButtonApply_clicked()
{
    auto stashName = listWidget->currentItem()->text();
    git()->applyStash(stashName);
}


void StashesWidget::on_pushButtonRemoveSelected_clicked()
{
    auto stashName = listWidget->currentItem()->text();
    git()->removeStash(stashName);
}


void StashesWidget::on_listWidget_itemActivated(QListWidgetItem *item)
{
    Q_UNUSED(item)

    pushButtonApply->setEnabled(true);
    pushButtonRemoveSelected->setEnabled(true);
}

