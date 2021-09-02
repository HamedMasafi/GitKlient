#ifndef ABSTRACTCOMMAND_H
#define ABSTRACTCOMMAND_H

#include <QByteArray>
#include <QStringList>
#include <QStringList>

class QCheckBox;
class QLineEdit;
class QWidget;

namespace Git {

class Manager;
class AbstractCommand
{
    bool _isValid{false};

protected:
    QStringList _args;
    Manager *_git;

public:
    enum Status {
        None,
        InvalidArgs,
        Ready,
        Running,
        Finished,
        Error
    };

    AbstractCommand();
    AbstractCommand(const QStringList &args);
    AbstractCommand(Manager *git);

    virtual ~AbstractCommand();

    virtual QStringList generateArgs() const = 0;
    virtual void parseOutput(const QByteArray &output, const QByteArray &errorOutput);

    bool isValid() const;

    virtual bool supportWidget() const { return false; }
    virtual bool supportProgress() const { return false; }
    QWidget *createWidget() const;
};

} // namespace Git

#endif // ABSTRACTCOMMAND_H
