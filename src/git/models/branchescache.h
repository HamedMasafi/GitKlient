#ifndef BRANCHESCACHE_H
#define BRANCHESCACHE_H

#include "cache.h"

#include <QStringList>

namespace Git {

class Manager;
class BranchesCache : public Cache
{
    Q_OBJECT


public:
    struct BranchData{
        QString name;
        int commitsAhead;
        int commitsBehind;
    };
    explicit BranchesCache(Manager *git, QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    BranchData *fromindex(const QModelIndex &index) const;

    const QString &currentBranch() const;
    const QString &referenceBranch() const;
    void setReferenceBranch(const QString &newReferenceBranch);

protected:
    void fill() override;

private:
    QList<BranchData*> _data;
    QString _currentBranch;
    QString _referenceBranch;
    void calculateCommitStats();

};

} // namespace Git

#endif // BRANCHESCACHE_H
