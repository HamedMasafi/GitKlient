#include "commandargsparser.h"
#include <QApplication>

CommandArgsParser::CommandArgsParser()
{

}

void CommandArgsParser::add(const QString &name, const CommandList &list)
{
    _commands.insert(name, list);
}

void CommandArgsParser::add(const QString &name, const QString &list)
{
    CommandList cmdList;
    auto parts = list.split(' ');
    for (auto &pp: parts) {
        auto p = pp;
        bool isOptional{false};
        if (p.startsWith("[") && p.endsWith("]")) {
            isOptional = true;
            p = p.mid(1, p.length() - 2);
        }

        if (p.startsWith("<") && p.endsWith(">"))
            cmdList.append({Command::Named, p.mid(1, p.length() - 2), isOptional});
        else
            cmdList.append({Command::Fixed, p, isOptional});
    }
    add(name, cmdList);
}

bool CommandArgsParser::check(const CommandList &commands)
{
    _params.clear();
    if (qApp->arguments().size() != commands.size() + 1)
        return false;
    auto appArgs = qApp->arguments();

    int idx{1};
    for (auto &cmd: commands) {
        switch (cmd.type) {
        case Command::Fixed:
            if (appArgs[idx] != cmd.s)
                return false;
            break;
        case Command::Named:
            _params.insert(cmd.s, appArgs[idx]);
            break;
        }
        idx++;
    }
    return true;
}

QString CommandArgsParser::checkAll()
{
    for (auto i = _commands.begin(); i != _commands.end(); ++i)
        if (check(i.value()))
            return i.key();
    return QString();
}

QString CommandArgsParser::param(const QString &name) const
{
    return _params.value(name);
}
