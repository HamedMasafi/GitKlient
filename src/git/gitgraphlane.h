#ifndef GITGRAPHLANE_H
#define GITGRAPHLANE_H

#include <QColor>

namespace Git {

namespace Impl {
struct LanesFactory;
}

class GraphLane
{

public:
    enum Type
    {
        None,
        Start,
        Pipe,
        Node,
        End,
        Transparent,
        Test
    };
    GraphLane();
    GraphLane(Type type);

    Type type() const;
    const QList<int> &bottomJoins() const;
    const QList<int> &upJoins() const;
    void setType(Type newType);

private:
    void generateRandomColor();

    Type _type{None};
    QList<int> _bottomJoins;
    QList<int> _upJoins;

    friend class LogList;
    friend struct LanesFactory;
    friend struct Impl::LanesFactory;
};
bool operator==(const GraphLane &, const GraphLane &);

}

#endif // GITGRAPHLANE_H
