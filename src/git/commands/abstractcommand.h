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
    False,
    Unset,
    True,
};

enum class FastForwardType {
    Unset,
    Yes,
    No,
    OnlyFastForward
};

OptionalBool checkStateToOptionalBool(Qt::CheckState checkState);

class Manager;
class AbstractCommand : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int progress READ progress WRITE setProgress NOTIFY progressChanged)


protected:
    QStringList _args;
    Manager *_git{nullptr};
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

    explicit AbstractCommand(QObject *parent = nullptr);
    explicit AbstractCommand(QStringList args);
    explicit AbstractCommand(Manager *git);

    virtual ~AbstractCommand();

    virtual QStringList generateArgs() const = 0;
    virtual void parseOutput(const QByteArray &output, const QByteArray &errorOutput);

    bool isValid() const;

    virtual bool supportWidget() const { return false; }
    virtual bool supportProgress() const { return false; }
    virtual QWidget *createWidget();
    int progress() const;

    Status status() const;
    void setStatus(Status newStatus);

    const QString &errorMessage() const;
    void setErrorMessage(const QString &newErrorMessage);

signals:
    void progressChanged(int progress);

private:
    bool _isValid{false};
    int m_progress{0};
    Status _status{None};
    QString _errorMessage;
};

} // namespace Git

#endif // ABSTRACTCOMMAND_H
