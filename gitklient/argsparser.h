#ifndef ARGSPARSER_H
#define ARGSPARSER_H

#include <QObject>

class ArgsParser : public QObject
{
    Q_OBJECT
public:
    explicit ArgsParser(QObject *parent = nullptr);

    void checkAll();

private slots:
    void run_diff(const QString &f1, const QString &f2);

private:
    void runMethod(const QMetaMethod &method);
};

#endif // ARGSPARSER_H
