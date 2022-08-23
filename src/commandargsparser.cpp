#include "commandargsparser.h"

#include "appwindow.h"
#include "dialogs/changedfilesdialog.h"
#include "dialogs/clonedialog.h"
#include "dialogs/commitpushdialog.h"
#include "dialogs/fileblamedialog.h"
#include "dialogs/filehistorydialog.h"
#include "dialogs/ignorefiledialog.h"
#include "dialogs/initdialog.h"
#include "dialogs/pulldialog.h"
#include "dialogs/runnerdialog.h"
#include "dialogs/taginfodialog.h"
#include "diffwindow.h"
#include "git/gitfile.h"
#include "git/gitmanager.h"
#include "git/models/logsmodel.h"
#include "mergewindow.h"

#include <QDebug>
#include <QApplication>
#include <QDir>
#include <QFileInfo>
#include <QMetaMethod>

#include <KMessageBox>
#include <klocalizedstring.h>

#define checkGitPath(path)                                                                         \
    do { QFileInfo fi(path);                                                                            \
    if (fi.isFile())                                                                               \
        git->setPath(fi.absolutePath());                                                           \
    else                                                                                           \
        git->setPath(path);                                                                        \
    if (!git->isValid()) {                                                                         \
        KMessageBox::sorry(nullptr, i18n("The path is not git repo: %1", path));                   \
        return 1;                                                                                  \
    } } while(false)

CommandArgsParser::CommandArgsParser() : QObject()
{
    git = Git::Manager::instance();
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

ArgParserReturn CommandArgsParser::run(const QStringList &args)
{
#define GET_OP(x) params.size() > x ? Q_ARG(QString, params.at(x)) : QGenericArgument()
    if (args.size() == 1)
        return main();
    auto name = QString(args.at(1)).replace("-", "_").toLocal8Bit();
    auto c = metaObject()->methodCount();
    qDebug() << "Running" << args;
    for(int i = 0; i < c; i++) {
        auto method = metaObject()->method(i);

        if (method.name().compare(name, Qt::CaseInsensitive) == 0) {
            if (method.parameterCount() != args.size() - 1) {
                auto params = args.mid(2);
                ArgParserReturn r;
                qDebug() << "Running:" << method.name();
                auto b = metaObject()->invokeMethod(this,
                                                    method.name(),
                                                    Q_RETURN_ARG(ArgParserReturn, r),
                                                    GET_OP(0),
                                                    GET_OP(1),
                                                    GET_OP(2),
                                                    GET_OP(3),
                                                    GET_OP(4),
                                                    GET_OP(5),
                                                    GET_OP(6),
                                                    GET_OP(7),
                                                    GET_OP(8),
                                                    GET_OP(9));

                if (!b) {
                    qDebug() << args.size() << method.parameterCount();
                }

                return r;
            }
        }
    }
#undef GET_OP
    qWarning().noquote() << "Method not found" << args.at(1);

    if (args.size() == 2)
        return main(args.at(1));
    return main();
}

ArgParserReturn CommandArgsParser::help()
{
    auto c = metaObject()->methodCount();

    for (auto i = metaObject()->classInfoOffset(); i < metaObject()->classInfoCount(); i++) {
        auto name = QString(metaObject()->classInfo(i).name());
        auto value = QString(metaObject()->classInfo(i).value());

        if (!name.startsWith("help."))
            continue;
        name = name.mid(5);

        _helpTexts.insert(name, value);
    }
    qDebug() << "Git Klient command line interface help:";
    for(int i = metaObject()->methodOffset(); i < c; i++) {
        auto method = metaObject()->method(i);
        qDebug().noquote() << "    " << method.name() << method.parameterNames().join(" ");
        qDebug().noquote() << _helpTexts.value(method.name());
    }
    return 0;
}

ArgParserReturn CommandArgsParser::clone(const QString &path)
{
    CloneDialog d;
    d.setLocalPath(path);

    if (d.exec() == QDialog::Accepted) {
        RunnerDialog r;

        auto cmd = d.command();
        r.run(cmd);
        r.exec();
        cmd->deleteLater();
    }
    return 0;
}

ArgParserReturn CommandArgsParser::init(const QString &path)
{
    InitDialog d(git);
    d.setPath(path);

    if (d.exec() == QDialog::Accepted) {
        QDir dir;
        if (!dir.mkpath(d.path())) {
            KMessageBox::sorry(nullptr,
                               i18n("Unable to create path: %1", d.path()),
                               i18n("Init repo"));
            return 1;
        }

        git->init(d.path());
        KMessageBox::information(nullptr, i18n("The repo inited successfully"));
    }
    return 0;
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

ArgParserReturn CommandArgsParser::push(const QString &path)
{
    QFileInfo fi(path);

    if (!fi.exists()) {
        return 0;
    }
    if (fi.isFile())
        git->setPath(fi.absolutePath());
    else
        git->setPath(fi.absoluteFilePath());
    CommitPushDialog d(git);
    d.exec();
    return 0;
}

ArgParserReturn CommandArgsParser::changes()
{
    QDir dir;
    git->setPath(dir.currentPath());
    ChangedFilesDialog d(git);
    d.exec();
    return 0;
}

ArgParserReturn CommandArgsParser::changes(const QString &path)
{
    QFileInfo fi(path);

    git->setPath(fi.isFile() ? fi.absoluteFilePath() : fi.absolutePath());
    ChangedFilesDialog d(git);
    d.exec();
    return 0;
}

ArgParserReturn CommandArgsParser::create_tag(const QString &path)
{
    checkGitPath(path);

    TagInfoDialog d(nullptr);
    d.setWindowTitle(i18n("New tag"));
    if (d.exec() == QDialog::Accepted) {
        git->createTag(d.tagName(), d.message());
    }
    return 0;
}

ArgParserReturn CommandArgsParser::diff()
{
    auto d = new DiffWindow();
    d->exec();
    return ExecApp;
}

ArgParserReturn CommandArgsParser::diff(const QString &file)
{
    QFileInfo fi(file);

    if (fi.isFile()) {
        git->setPath(fi.absolutePath());
        QDir dir(git->path());
        Git::File headFile(file);
        Git::File changedFile(git->currentBranch(), dir.relativeFilePath(file), git);
        auto d = new DiffWindow(headFile, changedFile);
        d->exec();
        return ExecApp;
    } else if (fi.isDir()) {
        git->setPath(fi.absoluteFilePath());
        auto d = new DiffWindow(git);
        d->exec();
        return ExecApp;
    }
    return 0;
}

ArgParserReturn CommandArgsParser::diff(const QString &file1, const QString &file2)
{
    qDebug() << file1 << file2;
    QFileInfo fi1(file1);
    QFileInfo fi2(file2);

    if (fi1.isFile() && fi2.isFile()) {
        qDebug() << fi1.absoluteFilePath() << fi2.absoluteFilePath();
        Git::File fileLeft(fi1.absoluteFilePath());
        Git::File fileRight(fi2.absoluteFilePath());
        auto d = new DiffWindow(fileLeft, fileRight);
        d->exec();
        return ExecApp;
    }
    if (fi1.isDir() && fi2.isDir()) {
        auto d = new DiffWindow(fi1.absoluteFilePath(), fi2.absoluteFilePath());
        d->exec();
        return ExecApp;
    }

    return 0;
}

ArgParserReturn CommandArgsParser::diff(const QString &path, const QString &file1, const QString &file2)
{
    if (file1.count(":") != 1 || file2.count(":") != 1)
        return 1;
    git->setPath(path);
    if (!git->isValid())
        return 1;
    auto parts1 = file1.split(":");
    auto parts2 = file2.split(":");
    Git::File fileLeft(parts1.first(), parts1.at(1));
    Git::File fileRight(parts2.first(), parts2.at(1));
    auto d = new DiffWindow(fileLeft, fileRight);
    d->exec();
    return ExecApp;
}

ArgParserReturn CommandArgsParser::blame(const QString &file)
{
    QFileInfo fi{file};

    if (!fi.exists() || !fi.isFile()) {
        return 0;
    }

    git->setPath(fi.absolutePath());
    git->setLoadFlags(Git::LoadNone);
    git->logsModel()->load();

    Git::File f(git->currentBranch(), file, git);
    FileBlameDialog d(f);
    d.exec();
    return 0;
}

ArgParserReturn CommandArgsParser::history(const QString &file)
{
    git->setPath(file.mid(0, file.lastIndexOf("/")));
    auto fileCopy = file;
    fileCopy = file.mid(git->path().size() + 1);
    FileHistoryDialog d(git, fileCopy);
    d.exec();
    return 0;
}

ArgParserReturn CommandArgsParser::merge()
{
    auto d = new MergeWindow;
    d->exec();
    return ExecApp;
}

ArgParserReturn CommandArgsParser::merge(const QString &base, const QString &local, const QString &remote, const QString &result)
{
    auto d = new MergeWindow;
    d->setFilePathLocal(local);
    d->setFilePathBase(base);
    d->setFilePathRemote(remote);
    d->setFilePathResult(result);
    d->load();
    int n = d->exec();

    if (n == QDialog::Accepted)
        return 0;
    else
        return 1;
}

ArgParserReturn CommandArgsParser::ignore(const QString &path)
{
    QFileInfo fi(path);
    if (!fi.exists())
        return 1;

    if (fi.isDir())
        git->setPath(path);
    else
        git->setPath(fi.absolutePath());

    if (!git->isValid())
        return 1;

    IgnoreFileDialog d(git, path);
    d.exec();
    return 0;
}

ArgParserReturn CommandArgsParser::add(const QString &path)
{
    checkGitPath(path);

    git->addFile(path);
    KMessageBox::information(nullptr, i18n("File(s) added to git successfully"));
    return 0;
}

ArgParserReturn CommandArgsParser::remove(const QString &path)
{
    checkGitPath(path);


    auto r = KMessageBox::questionYesNo(nullptr,
                                        i18n("Would you like to leave file(s) on disk?"));

    bool cached = r == KMessageBox::Yes;

    git->removeFile(path, cached);
    KMessageBox::information(nullptr, i18n("File(s) removed to git successfully"));
    return 0;
}

ArgParserReturn CommandArgsParser::main()
{
    auto window = AppWindow::instance();
    window->show();
    return ExecApp;
}

ArgParserReturn CommandArgsParser::main(const QString &path)
{
    auto window = new AppWindow(path);
    window->show();
    return ExecApp;
}
