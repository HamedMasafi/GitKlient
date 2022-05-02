#ifndef ADDSUBMODULECOMMAND_H
#define ADDSUBMODULECOMMAND_H

#include "abstractcommand.h"

namespace Git {

class AddSubmoduleCommand : public AbstractCommand
{
    Q_OBJECT

public:
    explicit AddSubmoduleCommand(Manager *git);

    QStringList generateArgs() const override;

    bool force() const;
    void setForce(bool newForce);

    QString branch() const;
    void setbranch(QString newbranch);

    const QString &url() const;
    void setUrl(const QString &newUrl);

    const QString &localPath() const;
    void setLocalPath(const QString &newLocalPath);

private:
    bool _force{false};
    QString _branch;
    QString _url;
    QString _localPath;
};

}

#endif // ADDSUBMODULECOMMAND_H
