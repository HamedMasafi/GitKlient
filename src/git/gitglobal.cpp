#include "gitglobal.h"

#include <QProcess>
#include "commands/abstractcommand.h"

namespace Git {

void run(const QString &workingDir, const QStringList &args)
{
    QProcess p;
    p.setProgram("git");
    p.setArguments(args);
    p.setWorkingDirectory(workingDir);
    p.start();
    p.waitForFinished();
//    auto ret = p.readAllStandardOutput() + p.readAllStandardError();

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
//    auto ret = p.readAllStandardOutput() + p.readAllStandardError();

    //    return ret;
}

QByteArray runGit(const QString &workingDir, const QStringList &args)
{
    //    qDebug().noquote() << "Running: git " << args.join(" ");

    QProcess p;
    p.setProgram("git");
    p.setArguments(args);
    p.setWorkingDirectory(workingDir);
    p.start();
    p.waitForFinished();
    auto out = p.readAllStandardOutput();
//    auto err = p.readAllStandardError();

    return out; // + err;
}

QStringList readAllNonEmptyOutput(const QString &workingDir, const QStringList &cmd, bool trim)
{
    QStringList list;
    auto out = QString(runGit(workingDir, cmd)).split("\n");

    for (auto &line : out) {
        QString b;

        if (trim)
            b = line.trimmed();
        else
            b = line;

        if (b.isEmpty())
            continue;

        list.append(b);
    }
    return list;
}

} // namespace Git
