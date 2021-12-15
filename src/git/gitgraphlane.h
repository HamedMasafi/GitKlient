#ifndef GITGRAPHLANE_H
#define GITGRAPHLANE_H

#include <QColor>

namespace Git {

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

//    GitGraphLane(GitGraphLane &&other);

    Type type() const;
    const QColor &color() const;


    int joinFrom() const;

    int joinTo() const;

private:
    void generateRandomColor();

    QColor _color;
    int _joinFrom{-1};
    int _joinTo{-1};
    Type _type{None};
    QList<int> _bottomJoins;
    QList<int> _upJoins;

    friend class LogList;
    friend struct LanesData;
    friend struct LanesFactory;
};
bool operator==(const GraphLane &, const GraphLane &);

}

#endif // GITGRAPHLANE_H
