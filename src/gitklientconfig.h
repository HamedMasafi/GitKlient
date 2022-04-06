#ifndef GITKLIENTCONFIG_H
#define GITKLIENTCONFIG_H

namespace Git {
class Manager;
}
class GitKlientConfig
{
    Git::Manager *_git;
public:
    GitKlientConfig(Git::Manager *git);

    void apply();
};

#endif // GITKLIENTCONFIG_H
