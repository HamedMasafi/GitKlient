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

private:
    Mode _mode;
    QString _target;

};

} // namespace Git

#endif // COMMANDSWITCHBRANCH_H
