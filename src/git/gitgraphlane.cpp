#include "gitgraphlane.h"

namespace Git {

GraphLane::GraphLane()
{
    generateRandomColor();
}

GraphLane::Type GraphLane::type() const
{
    return _type;
}

void GraphLane::generateRandomColor()
{
    //    auto r = QRandomGenerator::global();
    //    _color = QColor(r->bounded(0, 150), r->bounded(0, 150), r->bounded(0, 150));
}

void GraphLane::setType(Type newType)
{
    _type = newType;
}

const QList<int> &GraphLane::upJoins() const
{
    return _upJoins;
}

const QList<int> &GraphLane::bottomJoins() const
{
    return _bottomJoins;
}

GraphLane::GraphLane(GraphLane::Type type) : _type(type)
{
    generateRandomColor();
}

bool operator==(const GraphLane &p1, const GraphLane &p2)
{
    if (p1.type() == p2.type() && p1.type() == GraphLane::None)
        return true;

    return p1.type() == p2.type();
}

}
