#ifndef COMMANDSWITCHBRANCH_H
#define COMMANDSWITCHBRANCH_H

#include "abstractcommand.h"

namespace Git {

class CommandSwitchBranch : public AbstractCommand
{
    Q_OBJECT

public:
    CommandSwitchBranch(Manager *git);
    enum Mode { NewBranch, ExistingBranch, Tag };

    const QString &target() const;
    void setTarget(const QString &newTarget);

    Mode mode() const;
    void setMode(Mode newMode);

    QStringList generateArgs() const override;

    bool force() const;
    void setForce(bool newForce);

private:
    Mode _mode;
    QString _target;
    bool _force{false};

};

} // namespace Git

#endif // COMMANDSWITCHBRANCH_H
