#include "argsparser.h"
#include <QDebug>
#include <QApplication>
#include <QMetaMethod>

ArgsParser::ArgsParser(QObject *parent) : QObject(parent)
{

}

void ArgsParser::checkAll()
{
    if (qApp->arguments().size() < 2)
        return;

    auto group = qApp->arguments().at(1);

    auto c = "run_" + group;

    metaObject()->indexOfMethod(c.toLatin1().data());

    for (int i = 0; i < metaObject()->methodCount(); ++i) {
        auto method = metaObject()->method(i);
        if (method.name() != "run_" + group)
            return;
    }
}

void ArgsParser::run_diff(const QString &f1, const QString &f2)
{
    qDebug() << "diff";
}

void ArgsParser::runMethod(const QMetaMethod &method)
{
    int firstOptional{0};
    for (int i = 0; i < method.parameterCount(); ++i) {
//        if (method.parameterCount())
    }
}
