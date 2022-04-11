#include "cache.h"
#include "../gitmanager.h"

namespace Git {

Cache::Cache(Manager *git, QObject *parent) : QAbstractListModel(parent), _git(git)
{
//    connect(git, &Manager::pathChanged, this, &Cache::load);
}

bool Cache::isLoaded() const
{
    return m_status == Loaded;
}

Cache::Status Cache::status() const
{
    return m_status;
}

void Cache::load()
{
    if (!_git->isValid())
        return;

    setStatus(Loading);
    beginResetModel();
    fill();
    endResetModel();
    setStatus(Loaded);
}

void Cache::setStatus(Status newStatus)
{
    if (m_status == newStatus)
        return;
    m_status = newStatus;

    if (m_status == Loaded)
        emit loaded();

    emit statusChanged();
}

} // namespace Git