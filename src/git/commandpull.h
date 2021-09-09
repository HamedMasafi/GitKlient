#ifndef COMMANDPULL_H
#define COMMANDPULL_H

#include "abstractcommand.h"

namespace Git {

class CommandPull : public AbstractCommand
{    
public:
    CommandPull();
    QStringList generateArgs() const override;

    bool squash() const;
    void setSquash(bool newSquash);

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

private:
    bool _squash{false};
    bool _noFf{false};
    bool _ffOnly{false};
    bool _noCommit{false};
    bool _prune{false};
    bool _tags{false};
};

} // namespace Git

#endif // COMMANDPULL_H
