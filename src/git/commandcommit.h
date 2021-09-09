#ifndef COMMANDMESSAGE_H
#define COMMANDMESSAGE_H

#include "abstractcommand.h"

namespace Git {

class CommandCommit : public AbstractCommand
{
public:
    CommandCommit();
    QStringList generateArgs() const override;

    const QString &message() const;
    void setMessage(const QString &newMessage);

    bool amend() const;
    void setAmend(bool newAmend);

    bool includeStatus() const;
    void setIncludeStatus(bool newIncludeStatus);

private:
    QString _message;
    bool _amend{false};
    bool _includeStatus{false};
};

} // namespace Git

#endif // COMMANDMESSAGE_H
