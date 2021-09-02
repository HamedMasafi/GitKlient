#ifndef COMMANDARGSPARSER_H
#define COMMANDARGSPARSER_H

#include <QMap>
#include <QString>

struct Command {
    enum Type {
        Fixed,
        Named
    };

    Type type;
    QString s;
    bool isOptional;
};

using CommandList = QList<Command>;

class CommandArgsParser
{
    QMap<QString, CommandList> _commands;
    QMap<QString, QString> _params;

public:
    CommandArgsParser();
    void add(const QString &name, const CommandList &list);
    void add(const QString &name, const QString &list);
    bool check(const CommandList &commands);
    QString checkAll();
    QString param(const QString &name) const;
};

#endif // COMMANDARGSPARSER_H
