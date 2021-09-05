#include "dialogs/submoduleinfodialog.h"
#include "submoduleswidget.h"
#include "git/gitmanager.h"

#include <QDebug>

SubmodulesWidget::SubmodulesWidget(QWidget *parent) :
      WidgetBase(parent)
{
    setupUi(this);
}

SubmodulesWidget::SubmodulesWidget(Git::Manager *git, QWidget *parent) :
      WidgetBase(git, parent)
{
    setupUi(this);
}

void SubmodulesWidget::reload()
{
    auto modulesList = git()->submodules();

    auto changedFiles = git()->changedFiles();

    for (auto &m: modulesList) {
        auto item = new QTreeWidgetItem(treeWidget);
        item->setText(0,m.path());
        item->setText(1, m.refName());

        auto status = changedFiles.value(m.path());
        qDebug() << "status for" << m.path() << status;

        treeWidget->addTopLevelItem(item);
    }
}

void SubmodulesWidget::on_pushButtonAddNew_clicked()
{
    SubmoduleInfoDialog d(this);
    if (d.exec() == QDialog::Accepted) {
    }
}

