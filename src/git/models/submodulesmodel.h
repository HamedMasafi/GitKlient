#ifndef SUBMODULESCACHE_H
#define SUBMODULESCACHE_H

#include "abstractgititemsmodel.h"

namespace Git {

class Submodule;
class SubmodulesModel : public AbstractGitItemsModel
{
    Q_OBJECT

    QList<Submodule *> _data;

public:
    explicit SubmodulesModel(Git::Manager *git, QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    bool append(Submodule *module);
    Submodule *fromIndex(const QModelIndex &index);

protected:
    void fill() override;
};

} // namespace Git

#endif // SUBMODULESCACHE_H
