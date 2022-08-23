#ifndef COMMANDCHANGEDFILES_H
#define COMMANDCHANGEDFILES_H

#include "abstractcommand.h"
#include "../filestatus.h"

#include <QList>

namespace Git {

class CommandChangedFiles : public AbstractCommand
{

public:
    explicit CommandChangedFiles(Manager *git);

    const QList<FileStatus> &files() const;
    bool ignored() const;
    void setIgnored(bool newIgnored);
    bool untracked() const;
    void setUntracked(bool newUntracked);
    bool ignoreSubmodules() const;
    void setIgnoreSubmodules(bool newIgnoreSubmodules);

    QStringList generateArgs() const override;
    void parseOutput(const QByteArray &output, const QByteArray &errorOutput) override;

private:
    QList<FileStatus> _files;
    bool _ignored{false};
    bool _untracked{false};
    bool _ignoreSubmodules{true};
};

} // namespace Git

#endif // COMMANDCHANGEDFILES_H
