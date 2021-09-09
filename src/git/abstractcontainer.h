#ifndef ABSTRACTCONTAINER_H
#define ABSTRACTCONTAINER_H

#include <QList>

template <class T>
class AbstractContainer : public QList<T>
{
public:
    AbstractContainer()
    {

    }

    void reload() = 0;
    void clearAndDelete()
    {
        qDeleteAll(*this);
        this->clear();
    }
};

#endif // ABSTRACTCONTAINER_H
