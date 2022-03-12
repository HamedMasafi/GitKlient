#include "commandargsparser.h"

#include "commandargsparser.h"
#include "dialogs/changedfilesdialog.h"
#include "dialogs/fileblamedialog.h"
#include "dialogs/filehistorydialog.h"
#include "dialogs/pulldialog.h"
#include "dialogs/runnerdialog.h"
#include "diffwindow.h"
#include "git/gitfile.h"
#include "git/gitmanager.h"
#include "gitklientdebug.h"
#include "gitklientmergewindow.h"
#include "gitklientwindow.h"

#include <QApplication>
#include <QDir>
#include <QFileInfo>
#include <QMetaMethod>

CommandArgsParser::CommandArgsParser() : QObject()
{

}

void CommandArgsParser::add(const QString &name, const CommandList &list)
{
    _commands.insert(name, list);
}

void CommandArgsParser::add(const QString &name, const QString &list)
{
    CommandList cmdList;
    auto parts = list.split(' ');
    for (auto &pp: parts) {
        auto p = pp;
        bool isOptional{false};
        if (p.startsWith("[") && p.endsWith("]")) {
            isOptional = true;
            p = p.mid(1, p.length() - 2);
        }

        if (p.startsWith("<") && p.endsWith(">"))
            cmdList.append({Command::Named, p.mid(1, p.length() - 2), isOptional});
        else
            cmdList.append({Command::Fixed, p, isOptional});
    }
    add(name, cmdList);
}

bool CommandArgsParser::check(const CommandList &commands)
{
    _params.clear();
    if (qApp->arguments().size() != commands.size() + 1)
        return false;
    auto appArgs = qApp->arguments();

    int idx{1};
    for (auto &cmd: commands) {
        switch (cmd.type) {
        case Command::Fixed:
            if (appArgs[idx] != cmd.s)
                return false;
            break;
        case Command::Named:
            _params.insert(cmd.s, appArgs[idx]);
            break;
        }
        idx++;
    }
    return true;
}

QString CommandArgsParser::checkAll()
{
    for (auto i = _commands.begin(); i != _commands.end(); ++i)
        if (check(i.value()))
            return i.key();
    return QString();
}

QString CommandArgsParser::param(const QString &name) const
{
    return _params.value(name);
}

void CommandArgsParser::run(const QStringList &args)
{
    if (!args.size())
        return;
    auto name = args.first();
    auto c = metaObject()->methodCount();
    for(int i = 0; i < c; i++) {
        auto method = metaObject()->method(i);

        if (method.name() == name) {
            if (method.parameterCount() != args.size() - 1)
                continue;
        }
    }
}

ArgParserReturn CommandArgsParser::pull(const QString &path)
{
    git->setPath(path);
    PullDialog d;
    if (d.exec() == QDialog::Accepted) {
        RunnerDialog r;
        auto branch = git->currentBranch();
        r.run({"pull", "origin", branch});
        r.exec();
    }
    return 0;
}

ArgParserReturn CommandArgsParser::changes(const QString &path)
{
    QFileInfo fi(path);

    git->setPath(fi.isFile() ? fi.absoluteFilePath() : fi.absolutePath());
    ChangedFilesDialog d;
    d.exec();
    return 0;
}

ArgParserReturn CommandArgsParser::diff(const QString &file)
{
    auto d = new DiffWindow;
    d->show();
    return ExecApp;
}

ArgParserReturn CommandArgsParser::blame(const QString &file)
{
    Git::File f(git->currentBranch(), file, git);
    FileBlameDialog d(f);
    d.exec();
    return 0;
}

ArgParserReturn CommandArgsParser::history(const QString &file)
{
    QDir dir(git->path());
    git->setPath(file.mid(0, file.lastIndexOf("/")));
    auto fileCopy = file;
    fileCopy = file.mid(git->path().size() + 1);
    FileHistoryDialog d(git, fileCopy);
    d.exec();
    return 0;
}

ArgParserReturn CommandArgsParser::merge(const QString &base, const QString &local, const QString &remote, const QString &result)
{
    auto d = new GitKlientMergeWindow;
    d->show();
    return ExecApp;
}
