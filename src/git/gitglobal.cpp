#include "gitglobal.h"

#include <QProcess>
#include "git/commands/abstractcommand.h"

namespace Git {

void run(const QString &workingDir, const QStringList &args)
{
    QProcess p;
    p.setProgram("git");
    p.setArguments(args);
    p.setWorkingDirectory(workingDir);
    p.start();
    p.waitForFinished();
    auto ret = p.readAllStandardOutput() + p.readAllStandardError();

    //    return ret;
}
void run(const QString &workingDir, AbstractCommand &cmd)
{
    QProcess p;
    p.setProgram("git");
    p.setArguments(cmd.generateArgs());
    p.setWorkingDirectory(workingDir);
    p.start();
    p.waitForFinished();
    auto ret = p.readAllStandardOutput() + p.readAllStandardError();

    //    return ret;
}

} // namespace Git
