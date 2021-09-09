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

const QColor &GraphLane::color() const
{
    return _color;
}

void GraphLane::generateRandomColor()
{
    //    auto r = QRandomGenerator::global();
    //    _color = QColor(r->bounded(0, 150), r->bounded(0, 150), r->bounded(0, 150));
}

int GraphLane::joinTo() const
{
    return _joinTo;
}

int GraphLane::joinFrom() const
{
    return _joinFrom;
}

GraphLane::GraphLane(GraphLane::Type type) : _type(type)
{
    generateRandomColor();
}

//GitGraphLane::GitGraphLane(GitGraphLane &&other)
//    : _color(std::move(other._color))
//    , _joinCol(std::move(other._joinCol))
//    , _type(std::move(other._type))
//{

//}

inline bool operator==(const GraphLane &p1, const GraphLane &p2)
{
    if (p1.type() == p2.type() && p1.type() == GraphLane::None)
        return true;

    return p1.type() == p2.type() && p1.joinFrom() == p2.joinFrom() && p1.joinTo() == p2.joinTo();
}

}