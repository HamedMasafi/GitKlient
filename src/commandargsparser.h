#ifndef COMMANDARGSPARSER_H
#define COMMANDARGSPARSER_H

#include <QMap>
#include <QObject>
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

enum ParseReturnType {
    ExecApp,
    ReturnCode
};

struct ArgParserReturn {
    ParseReturnType type;
    int code{0};

    ArgParserReturn(){}
    ArgParserReturn(int code) : type{ReturnCode}, code{code} {}
    ArgParserReturn(ParseReturnType type) : type{type}, code{0} {}
};

namespace Git {
class Manager;
}

using CommandList = QList<Command>;

#define HelpText(name, text) Q_CLASSINFO("help." #name, text);
class CommandArgsParser : public QObject
{
    Q_OBJECT

    QMap<QString, CommandList> _commands;
    QMap<QString, QString> _params;
    Git::Manager *git;
    QMap<QString, QString> _helpTexts;

    HelpText(clone, "Clone an repo")

public:
    CommandArgsParser();
    void add(const QString &name, const CommandList &list);
    void add(const QString &name, const QString &list);
    bool check(const CommandList &commands);
    QString checkAll();
    QString param(const QString &name) const;
    ArgParserReturn run(const QStringList &args);

public slots:
    ArgParserReturn help();

    ArgParserReturn clone(const QString &path);
    ArgParserReturn init(const QString &path);
    ArgParserReturn pull(const QString &path);
    ArgParserReturn push(const QString &path);
    ArgParserReturn changes();
    ArgParserReturn changes(const QString &path);

    ArgParserReturn create_tag(const QString &path);
    ArgParserReturn diff();
    ArgParserReturn diff(const QString &file);
    ArgParserReturn diff(const QString &file1, const QString &file2);
    ArgParserReturn diff(const QString &path, const QString &file1, const QString &file2);
    ArgParserReturn blame(const QString &file);
    ArgParserReturn history(const QString &file);
    ArgParserReturn merge();
    ArgParserReturn merge(const QString &base,
              const QString &local,
              const QString &remote,
              const QString &result);
    ArgParserReturn ignore(const QString &path);

    ArgParserReturn add(const QString &path);
    ArgParserReturn remove(const QString &path);

    ArgParserReturn main();
    ArgParserReturn main(const QString &path);
};

#endif // COMMANDARGSPARSER_H
