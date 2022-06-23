#include "appconfig.h"
#include "git/gitmanager.h"

AppConfig::AppConfig(Git::Manager *git) : _git(git)
{

}

void AppConfig::apply()
{
    _git->setConfig("alias.klient", "!gitklient", Git::Manager::ConfigGlobal);
    _git->setConfig("alias.gkdiff",
                    "'difftool --dir-diff --tool=gitklientdiff'",
                    Git::Manager::ConfigGlobal);

    _git->setConfig("mergetool.gitklientmerge.cmd",
                    "gitklientmerge \"$BASE\" \"$LOCAL\" \"$REMOTE\" \"$MERGED\"",
                    Git::Manager::ConfigGlobal);
    _git->setConfig("mergetool.gitklientmerge.trustExitCode", "true", Git::Manager::ConfigGlobal);

    _git->setConfig("difftool.gitklientdiff.cmd",
                    "gitklientdiff \"$LOCAL\" \"$REMOTE\"",
                    Git::Manager::ConfigGlobal);

}
