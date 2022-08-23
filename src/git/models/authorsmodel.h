#ifndef AUTHORSMODEL_H
#define AUTHORSMODEL_H

#include "abstractgititemsmodel.h"

namespace Git {

struct Author {
    QString email;
    QString name;
};

class __attribute__((unused)) AuthorsModel : public AbstractGitItemsModel
{
    Q_OBJECT
    QList<Author *> _data;

public:
    explicit AuthorsModel(Manager *git, QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    QVariant data(const QModelIndex &index, int role) const override;

    Author *findOrCreate(const QString &name, const QString &email);

protected:
    void fill() override;
};

} // namespace Git

#endif // AUTHORSMODEL_H
