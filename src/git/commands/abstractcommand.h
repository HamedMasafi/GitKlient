#ifndef ABSTRACTCOMMAND_H
#define ABSTRACTCOMMAND_H

#include <QByteArray>
#include <QObject>
#include <QStringList>
#include <QStringList>

class QCheckBox;
class QLineEdit;
class QWidget;

namespace Git {

enum class OptionalBool {
    Unset,
    True,
    False
};

enum class FastForwardType {
    Unset,
    Yes,
    No,
    OnlyFastForward
};

class Manager;
class AbstractCommand : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int progress READ progress WRITE setProgress NOTIFY progressChanged)

    bool _isValid{false};

    int m_progress{0};

protected:
    QStringList _args;
    Manager *_git;
    void setProgress(int newProgress);
    void appendBool(OptionalBool b, QStringList &cmd, const QString &name) const;
    void appendBool(bool b, QStringList &cmd, const QString &name) const;

public:
    enum Status {
        None,
        InvalidArgs,
        Ready,
        Running,
        Finished,
        Error
    };

    AbstractCommand(QObject *parent = nullptr);
    AbstractCommand(const QStringList &args);
    AbstractCommand(Manager *git);

    virtual ~AbstractCommand();

    virtual QStringList generateArgs() const = 0;
    virtual void parseOutput(const QByteArray &output, const QByteArray &errorOutput);

    bool isValid() const;

    virtual bool supportWidget() const { return false; }
    virtual bool supportProgress() const { return false; }
    virtual QWidget *createWidget();
    int progress() const;
signals:
    void progressChanged(int progress);
};

} // namespace Git

#endif // ABSTRACTCOMMAND_H
