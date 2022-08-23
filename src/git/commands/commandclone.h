#ifndef CLONECOMMAND_H
#define CLONECOMMAND_H

#include "abstractcommand.h"

namespace Git {

class CloneCommand : public AbstractCommand
{
    Q_OBJECT

public:
    explicit CloneCommand(QObject *parent = nullptr);

    const QString &repoUrl() const;
    void setRepoUrl(const QString &newRepoUrl);
    const QString &localPath() const;
    void setLocalPath(const QString &newLocalPath);
    const QString &branch() const;
    void setBranch(const QString &newBranch);
    int depth() const;
    void setDepth(int newDepth);
    const QString &origin() const;
    void setOrigin(const QString &newOrigin);
    bool recursive() const;
    void setRecursive(bool newRecursive);

private:
    QString _repoUrl;
    QString _localPath;
    QString _branch;
    int _depth{-1};
    QString _origin;
    bool _recursive{false};

    // AbstractCommand interface
public:
    QStringList generateArgs() const override;
    void parseOutput(const QByteArray &output, const QByteArray &errorOutput) override;
    bool supportProgress() const override;
};

} // namespace Git

#endif // CLONECOMMAND_H
