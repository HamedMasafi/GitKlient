#ifndef COMMANDADDREMOTE_H
#define COMMANDADDREMOTE_H

#include "abstractcommand.h"

namespace Git {

class CommandAddRemote : public AbstractCommand
{
public:
    explicit CommandAddRemote(QObject *parent = nullptr);

    QStringList generateArgs() const override;


    const QString &remoteName() const;
    void setRemoteName(const QString &newRemoteName);
    OptionalBool tags() const;
    void setTags(OptionalBool newTags);
    void setTags(Qt::CheckState newTags);
    bool mirror() const;
    void setMirror(bool newMirror);
    const QString &master() const;
    void setMaster(const QString &newMaster);
    bool fetch() const;
    void setFetch(bool newFetch);

    const QString &url() const;
    void setUrl(const QString &newUrl);

private:
    QString _remoteName;
    OptionalBool _tags{OptionalBool::False};
    bool _mirror{false};
    QString _master;
    bool _fetch{false};
    QString _url;
};

}

#endif // COMMANDADDREMOTE_H
