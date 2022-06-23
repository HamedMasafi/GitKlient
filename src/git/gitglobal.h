#ifndef GITGLOBAL_H
#define GITGLOBAL_H

#include "commands/abstractcommand.h"

#include <QStringList>

namespace Git {
void run(const QString &workingDir, const QStringList& args);
void run(const QString &workingDir, AbstractCommand& cmd);

QByteArray runGit(const QString &workingDir, const QStringList &args);
QStringList readAllNonEmptyOutput(const QString &workingDir, const QStringList &cmd, bool trim = true);
} // namespace Git

#endif // GITGLOBAL_H
