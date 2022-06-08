#ifndef CommandFetch_H
#define CommandFetch_H

#include "abstractcommand.h"

namespace Git {

class CommandFetch : public AbstractCommand
{
public:
    CommandFetch();

    QStringList generateArgs() const override;

    bool noFf() const;
    void setNoFf(bool newNoFf);

    bool ffOnly() const;
    void setFfOnly(bool newFfOnly);

    bool noCommit() const;
    void setNoCommit(bool newNoCommit);

    bool prune() const;
    void setPrune(bool newPrune);

    bool tags() const;
    void setTags(bool newTags);

    void parseOutput(const QByteArray &output, const QByteArray &errorOutput) override;

    const QString &remote() const;
    void setRemote(const QString &newRemote);

    const QString &branch() const;
    void setBranch(const QString &newBranch);

private:
    bool _noFf{false};
    bool _ffOnly{false};
    bool _noCommit{false};
    bool _prune{false};
    bool _tags{false};
    QString _remote;
    QString _branch;
};

} // namespace Git

#endif // CommandFetch_H
