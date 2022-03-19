#include "modificationsdialog.h"
#include "git/gitmanager.h"

ModificationsDialog::ModificationsDialog(QWidget *parent) :
      Dialog(parent)
{
    setupUi(this);

    auto git = Git::Manager::instance();

    auto files = git->changedFiles();

    for (auto i = files.begin(); i != files.end(); ++i) {
        auto item = new QListWidgetItem(listWidget);
        item->setText(i.key());
        switch (i.value()) {
        case Git::Manager::Modified:
            item->setForeground(Qt::blue);
            break;
        case Git::Manager::Added:
            item->setForeground(Qt::green);
            break;
        case Git::Manager::Removed:
            item->setForeground(Qt::red);
            break;

        default:
            break;
        }
        item->setCheckState(Qt::Unchecked);
        item->setData(1, i.value());
        listWidget->addItem(item);
    }
}
