#include "commandchangedfiles.h"
#include "../gitmanager.h"

namespace Git {

CommandChangedFiles::CommandChangedFiles(Manager *git) : AbstractCommand(git)
{

}

const QList<FileStatus> &CommandChangedFiles::files() const
{
    return _files;
}

bool CommandChangedFiles::ignored() const
{
    return _ignored;
}

void CommandChangedFiles::setIgnored(bool newIgnored)
{
    _ignored = newIgnored;
}

bool CommandChangedFiles::untracked() const
{
    return _untracked;
}

void CommandChangedFiles::setUntracked(bool newUntracked)
{
    _untracked = newUntracked;
}

bool CommandChangedFiles::ignoreSubmodules() const
{
    return _ignoreSubmodules;
}

void CommandChangedFiles::setIgnoreSubmodules(bool newIgnoreSubmodules)
{
    _ignoreSubmodules = newIgnoreSubmodules;
}

QStringList CommandChangedFiles::generateArgs() const
{
    QStringList args{"status",
                     "--short",
                     "--porcelain"};
    if (_untracked)
        args.append("--untracked-files=all");

    if (_ignoreSubmodules)
        args.append("--ignore-submodules");

    if (_ignored)
        args.append("--ignored");

    return args;
}

void CommandChangedFiles::parseOutput(const QByteArray &output, const QByteArray &errorOutput)
{
    Q_UNUSED(errorOutput)
    auto buffer = QString(output).split("\n");

    for (auto &item: buffer) {
        if (!item.trimmed().size())
            continue;
        FileStatus fs;
        fs.parseStatusLine(item);
        fs.setFullPath(_git->path() + fs.name());
        _files.append(fs);
    }
}

} // namespace Git
