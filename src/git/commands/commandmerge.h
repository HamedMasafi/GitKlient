#ifndef COMMANDMERGE_H
#define COMMANDMERGE_H

#include "abstractcommand.h"

namespace Git {

class CommandMerge : public AbstractCommand
{
    Q_OBJECT

public:
    explicit CommandMerge(Manager *git);
    QStringList generateArgs() const override;

    OptionalBool commit() const;
    void setCommit(OptionalBool newCommit);

    OptionalBool allowUnrelatedHistories() const;
    void setAllowUnrelatedHistories(OptionalBool newAllowUnrelatedHistories);

    FastForwardType ff() const;
    void setFf(FastForwardType newFf);

    bool squash() const;
    void setSquash(bool newSquash);

    const QString &fromBranch() const;
    void setFromBranch(const QString &newFromBranch);

private:
    OptionalBool _commit{OptionalBool::Unset};
    OptionalBool _allowUnrelatedHistories{OptionalBool::Unset};
    FastForwardType _ff;
    bool _squash{false};
    QString _fromBranch;
};

}

#endif // COMMANDMERGE_H
