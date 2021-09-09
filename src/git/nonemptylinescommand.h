#ifndef NONEMPTYLINESCOMMAND_H
#define NONEMPTYLINESCOMMAND_H

#include "parameterescommand.h"

namespace Git {

class NonEmptyLinesCommand : public ParameteresCommand
{
    QStringList _lines;

public:
    NonEmptyLinesCommand();

    void parseOutput(const QByteArray &output, const QByteArray &errorOutput) override;
    const QStringList &lines() const;
};

} // namespace Git

#endif // NONEMPTYLINESCOMMAND_H
