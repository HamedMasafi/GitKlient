#ifndef TAGSMODEL_H
#define TAGSMODEL_H

#include "abstractgititemsmodel.h"

namespace Git {

class Tag;
class TagsModel : public AbstractGitItemsModel
{
    Q_OBJECT

    QList<Tag *> _data;

public:
    TagsModel(Manager *git, QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Tag *fromindex(const QModelIndex &index) const;

protected:
    void fill() override;
};

} // namespace Git

#endif // TAGSMODEL_H
