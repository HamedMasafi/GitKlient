#ifndef COMMANDPUSH_H
#define COMMANDPUSH_H

#include "abstractcommand.h"

namespace Git {

class CommandPush : public AbstractCommand
{

public:
    CommandPush();
    QStringList generateArgs() const override;

    bool force() const;
    void setForce(bool newForce);

    const QString &remote() const;
    void setRemote(const QString &newRemote);

    const QString &localBranch() const;
    void setLocalBranch(const QString &newBranch);

    const QString &remoteBranch() const;
    void setRemoteBranch(const QString &newRemoteBranch);

private:
    QString _remote;
    QString _localBranch;
    QString _remoteBranch;
    bool _force{false};
};

} // namespace Git

#endif // COMMANDPUSH_H
