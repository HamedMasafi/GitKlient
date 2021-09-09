#include "nonemptylinescommand.h"

#include <QString>

namespace Git {

const QStringList &NonEmptyLinesCommand::lines() const
{
    return _lines;
}

NonEmptyLinesCommand::NonEmptyLinesCommand() : ParameteresCommand()
{

}

void NonEmptyLinesCommand::parseOutput(const QByteArray &output, const QByteArray &errorOutput)
{
    Q_UNUSED(errorOutput)
    QStringList list;
    auto out = QString(output).split("\n");

    for (auto &line: out) {
        auto b = line.trimmed();
        if (b.isEmpty())
            continue;

        _lines.append(b.trimmed());
    }
}

} // namespace Git
