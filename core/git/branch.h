#ifndef BRANCH_H
#define BRANCH_H

#include <QString>

class Branch
{
public:
    Branch();

private:
    enum class Status
    {

    };
    QString _localName;
    QString _remoteName;
};

#endif // BRANCH_H
