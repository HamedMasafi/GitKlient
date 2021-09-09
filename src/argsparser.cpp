#include "argsparser.h"
#include "dialogs/changedfilesdialog.h"
#include "diffwindow.h"
#include "git/gitmanager.h"

#include <QApplication>
#include <QDebug>
#include <QMetaMethod>

ArgsParser::ArgsParser(QObject *parent) : QObject(parent)
{
    git = Git::Manager::instance();
}

void ArgsParser::checkAll()
{
    if (qApp->arguments().size() < 2)
        return;

    auto group = qApp->arguments().at(1);

    auto c = "run_" + group;

    auto index = metaObject()->indexOfMethod(c.toLatin1().data());

    if (index == -1)
        return;

//    auto method = metaObject()->method(index);

    QList<QGenericArgument> args;
    for (int i = 2; i < qApp->arguments().size(); ++i)
        args.append(Q_ARG(QString, qApp->arguments().at(i)));

    metaObject()->invokeMethod(this,
                               c.toLatin1().data(),
                               args.value(0, QGenericArgument()),
                               args.value(1, QGenericArgument()),
                               args.value(2, QGenericArgument()),
                               args.value(3, QGenericArgument()),
                               args.value(4, QGenericArgument()),
                               args.value(5, QGenericArgument()),
                               args.value(6, QGenericArgument()),
                               args.value(7, QGenericArgument()),
                               args.value(8, QGenericArgument()));
}

void ArgsParser::run_diff(const QString &f1, const QString &f2)
{
    qDebug() << "diff" << f1 << f2;
    auto d = new DiffWindow;
    d->show();
}

void ArgsParser::run_changes(const QString &path)
{
    git->setPath(path);
    ChangedFilesDialog d;
    d.exec();
}

void ArgsParser::runMethod(const QMetaMethod &method)
{
//    int firstOptional{0};
    for (int i = 0; i < method.parameterCount(); ++i) {
//        if (method.parameterCount())
    }
}
