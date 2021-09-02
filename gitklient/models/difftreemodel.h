#ifndef DIFFTREEMODEL_H
#define DIFFTREEMODEL_H

#include "git/filestatus.h"
#define TREEMODEL_NODE_DATA_TYPE FileStatus::Status
#include "treemodel.h"

class DiffTreeModel : public TreeModel
{
    Q_OBJECT

public:
    DiffTreeModel(QObject *parent = nullptr);

    void addFile(const FileStatus& file);

    QVariant data(const QModelIndex &index, int role) const;

private:
    Node *createPath(const QStringList &path, const FileStatus::Status &status);
    QColor statusColor(const FileStatus::Status &status) const;
};

#endif // DIFFTREEMODEL_H
