#include "graphpainter.h"

#include <QPainter>
#include <QDebug>
#include <QPainterPath>
#include "models/historymodel.h"
#include "git/gitlog.h"

#define HEIGHT 30

GraphPainter::GraphPainter(HistoryModel *model, QObject *parent)
    : QStyledItemDelegate(parent), _model(model)
{
    _colors = {
        Qt::red, Qt::blue, Qt::green, Qt::magenta, Qt::darkMagenta, Qt::darkBlue, Qt::darkBlue,
        Qt::darkRed, Qt::darkYellow, Qt::darkGreen
    };
}

void GraphPainter::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

    auto log = _model->log(index);


    painter->setRenderHints(QPainter::Antialiasing);

//    painter->fillRect(option.rect, option.palette.base());
    if (option.state & QStyle::State_Selected)
        painter->fillRect(option.rect, option.palette.highlight());
    else if (option.state & QStyle::State_MouseOver)
        painter->fillRect(option.rect, option.palette.brush(QPalette::Normal, QPalette::Highlight));
//    else if (index.row() & 1)
//        painter->fillRect(option.rect, option.palette.alternateBase());
    else
        painter->fillRect(option.rect, option.palette.base());

    painter->save();
    painter->setClipRect(option.rect, Qt::IntersectClip);
    painter->translate(option.rect.topLeft());

    int x{-1};
    for (auto &l: log->lanes()) {
        ++x;
        if (l.type() == Git::GraphLane::None)
            continue;

        if (x >= _colors.size()) {
            painter->setPen(Qt::black);
            painter->setBrush(Qt::black);
        } else {
            painter->setPen(_colors.at(x));
            painter->setBrush(_colors.at(x));
        }
//        painter->setPen(l.color());
//        painter->setBrush(l.color());
        paintLane(painter, l, x);
    }
    painter->restore();

}

QPoint center(int x) {
    return {(x * HEIGHT) + (HEIGHT / 2), HEIGHT / 2};
}
QPoint centerEdge(int x, const Qt::Edge &edge) {
    switch (edge) {
    case Qt::TopEdge:
        return {(x * HEIGHT) + (HEIGHT / 2), 0};
    case Qt::LeftEdge:
        return {(x * HEIGHT), HEIGHT / 2};
    case Qt::RightEdge:
        return {(x + 1) * HEIGHT, HEIGHT / 2};
    case Qt::BottomEdge:
        return {(x * HEIGHT) + (HEIGHT / 2), HEIGHT};
    }
    return QPoint();
}

QPoint point(int col, const Qt::Alignment &align = Qt::AlignCenter) {
    int y;
    int x = col * HEIGHT;

    if (align & Qt::AlignTop)
        y = 0;
    else if (align & Qt::AlignBottom)
        y = HEIGHT;
    else
        y = HEIGHT / 2;

    if (align & Qt::AlignLeft)
        x += 0;
    else if (align & Qt::AlignRight)
        x += HEIGHT;
    else
        x += HEIGHT / 2;

    return {x, y};
}
QPoint centerGuide(int x, const Qt::Edge &edge) {
    constexpr const int tel{5};

    QPoint pt = point(x);
    switch (edge) {
    case Qt::TopEdge:
        pt.setY(pt.y() - tel);
        break;
    case Qt::LeftEdge:
        pt.setX(pt.x() - tel);
        break;
    case Qt::RightEdge:
        pt.setX(pt.x() + tel);
        break;
    case Qt::BottomEdge:
        pt.setY(pt.y() + tel);
        break;
    }
    return pt;
}

void GraphPainter::paintLane(QPainter *painter, const Git::GraphLane &lane, int index) const
{

    switch (lane.type()) {
    case Git::GraphLane::Start:
        if (lane.joinTo() == -1) {
            painter->drawLine(point(index), point(index, Qt::AlignTop));
            painter->setBrush(Qt::white);
            painter->drawEllipse(point(index), 3, 3);
        }
        break;
    case Git::GraphLane::Pipe:
        painter->drawLine(point(index, Qt::AlignTop), point(index, Qt::AlignBottom));
        break;
    case Git::GraphLane::Node:
        painter->drawLine(point(index, Qt::AlignTop), point(index, Qt::AlignBottom));
        painter->setBrush(Qt::white);
        painter->drawEllipse(point(index), 3, 3);
        break;
    case Git::GraphLane::End:
        if (lane.joinFrom() == -1) {
            painter->drawLine(point(index), point(index, Qt::AlignBottom));
            painter->setBrush(Qt::white);
            painter->drawEllipse(point(index), 3, 3);
        }
        break;
    case Git::GraphLane::Test:
        painter->drawLine(point(index, Qt::AlignTop | Qt::AlignLeft),
                          point(index, Qt::AlignBottom | Qt::AlignRight));
        painter->drawLine(point(index, Qt::AlignTop | Qt::AlignRight),
                          point(index, Qt::AlignBottom | Qt::AlignLeft));
        break;

    case Git::GraphLane::None:
    case Git::GraphLane::Transparent:
        break;//just to avoid compiler warning
    }

    if (lane.joinTo()!=-1) {
        QPainterPath p;
        p.moveTo(point(lane.joinTo()));
        p.cubicTo(centerGuide(index, Qt::LeftEdge),
                  centerGuide(index, Qt::TopEdge),
                  point(index, Qt::AlignTop));
        painter->setBrush(Qt::transparent);
        painter->drawPath(p);
        if (lane.type() != Git::GraphLane::End)
            qDebug() << "Invalid lane" << lane.type()<<lane.type();
    }
    if (lane.joinFrom() != -1) {
        QPainterPath p;
        p.moveTo(point(index, Qt::AlignBottom));
        p.cubicTo(centerGuide(index, Qt::BottomEdge),
                  centerGuide(index, Qt::LeftEdge),
                  point(lane.joinFrom()));
        painter->setBrush(Qt::transparent);
//        painter->setPen(Qt::DotLine);
        painter->drawPath(p);
    }
}

int GraphPainter::colX(const int &col) const
{
    return col * HEIGHT;
}

QSize GraphPainter::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option)
    Q_UNUSED(index)
    return {0, HEIGHT};
}
