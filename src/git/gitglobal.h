#ifndef GITGLOBAL_H
#define GITGLOBAL_H

#include "abstractcommand.h"

#include <QStringList>

namespace Git {
void run(const QString &workingDir, const QStringList& args);
void run(const QString &workingDir, AbstractCommand& cmd);
} // namespace Git

#endif // GITGLOBAL_H
