#include "remoteinfodialog.h"
#include "remoteswidget.h"
#include "git/gitmanager.h"
#include "git/models/remotesmodel.h"
#include "actions/remotesactions.h"


RemotesWidget::RemotesWidget(QWidget *parent) : WidgetBase(parent)
{
    setupUi(this);
    init(Git::Manager::instance());
}

RemotesWidget::RemotesWidget(Git::Manager *git, AppWindow *parent) :
      WidgetBase(git, parent)
{
    setupUi(this);

    init(git);
}

void RemotesWidget::saveState(QSettings &settings) const
{
    save(settings, splitter);
    save(settings, treeWidget);
}

void RemotesWidget::restoreState(QSettings &settings)
{
    restore(settings, splitter);
    restore(settings, treeWidget);
}

void RemotesWidget::on_listView_itemActivated(const QModelIndex &index)
{
    auto remote = _model->fromIndex(index);
    if (!remote)
        return;

    _actions->setRemoteName(remote->name);
    labelRemoteName->setText(remote->name);
    labelFetchUrl->setText(remote->fetchUrl);
    labelPushUrl->setText(remote->pushUrl);
    labelDefaultBranch->setText(remote->headBranch);
    treeWidget->clear();

    for (const auto &rb: qAsConst(remote->branches)) {
        auto item = new QTreeWidgetItem(treeWidget);

        item->setText(0, rb.name);
        item->setText(1, rb.remotePushBranch);
        item->setText(2, rb.remotePullBranch);
        item->setText(3, rb.statusText());

        treeWidget->addTopLevelItem(item);
    }
}

void RemotesWidget::on_listView_customContextMenuRequested(const QPoint &pos)
{
    Q_UNUSED(pos)
    auto remote = _model->fromIndex(listView->currentIndex());
    if (!remote)
        return;

    _actions->setRemoteName(remote->name);
    _actions->popup();
}

void RemotesWidget::init(Git::Manager *git)
{
    _model = git->remotesModel();
    listView->setModelColumn(0);
    listView->setModel(_model);

    _actions = new RemotesActions(git, this);
    pushButtonAdd->setAction(_actions->actionCreate());
    pushButtonRemove->setAction(_actions->actionRemove());
}
