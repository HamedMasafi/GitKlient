#ifndef PARAMETERESCOMMAND_H
#define PARAMETERESCOMMAND_H

#include "commands/abstractcommand.h"

namespace Git {

class ParameteresCommand : public AbstractCommand
{
    QStringList _args;

public:
    ParameteresCommand();
    const QStringList &args() const;
    void setArgs(const QStringList &newArgs);
    void appendArg(const QString &arg);

    QStringList generateArgs() const override;
};

} // namespace Git

#endif // PARAMETERESCOMMAND_H
