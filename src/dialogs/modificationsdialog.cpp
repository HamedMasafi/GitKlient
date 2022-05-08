#include "modificationsdialog.h"
#include "git/gitmanager.h"
#include "GitKlientSettings.h"

ModificationsDialog::ModificationsDialog(QWidget *parent) :
      AppDialog(parent)
{
    setupUi(this);

    auto git = Git::Manager::instance();

    auto files = git->changedFiles();

    for (auto i = files.begin(); i != files.end(); ++i) {
        auto item = new QListWidgetItem(listWidget);
        item->setText(i.key());
        switch (i.value()) {
        case Git::Manager::Modified:
            item->setForeground(GitKlientSettings::diffModifiedColor());
            break;
        case Git::Manager::Added:
            item->setForeground(GitKlientSettings::diffAddedColor());
            break;
        case Git::Manager::Removed:
            item->setForeground(GitKlientSettings::diffRemovedColor());
            break;

        default:
            break;
        }
        item->setCheckState(Qt::Unchecked);
        item->setData(1, i.value());
        listWidget->addItem(item);
    }
}
