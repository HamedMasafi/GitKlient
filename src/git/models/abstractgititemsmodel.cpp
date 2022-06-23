#include "abstractgititemsmodel.h"
#include "../gitmanager.h"

namespace Git {

AbstractGitItemsModel::AbstractGitItemsModel(Manager *git, QObject *parent) : QAbstractListModel(parent), _git(git)
{
//    connect(git, &Manager::pathChanged, this, &Cache::load);
}

bool AbstractGitItemsModel::isLoaded() const
{
    return m_status == Loaded;
}

AbstractGitItemsModel::Status AbstractGitItemsModel::status() const
{
    return m_status;
}

void AbstractGitItemsModel::load()
{
    if (!_git->isValid())
        return;

    setStatus(Loading);
    beginResetModel();
    fill();
    endResetModel();
    setStatus(Loaded);
}

void AbstractGitItemsModel::setStatus(Status newStatus)
{
    if (m_status == newStatus)
        return;
    m_status = newStatus;

    if (m_status == Loaded)
        emit loaded();

    emit statusChanged();
}

} // namespace Git
