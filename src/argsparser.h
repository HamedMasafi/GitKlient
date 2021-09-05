#ifndef ARGSPARSER_H
#define ARGSPARSER_H

#include <QObject>

namespace Git {
class Manager;
}
class ArgsParser : public QObject
{
    Q_OBJECT
        Git::Manager *git;
public:
    explicit ArgsParser(QObject *parent = nullptr);

    void checkAll();

private slots:
    void run_diff(const QString &f1, const QString &f2);
    void run_changes(const QString &path);
private:
    void runMethod(const QMetaMethod &method);
};

#endif // ARGSPARSER_H
