#ifndef GRAPHPAINTER_H
#define GRAPHPAINTER_H

#include <QStyledItemDelegate>
#include "git/gitgraphlane.h"

class HistoryModel;
class GraphPainter : public QStyledItemDelegate
{
    Q_OBJECT
    HistoryModel *_model;
    QVector<QColor> _colors;

public:
    GraphPainter(HistoryModel *model, QObject *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void paintLane(QPainter *painter, const Git::GraphLane &lane, int index) const;
    int colX(const int &col) const;

    // QAbstractItemDelegate interface
public:
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // GRAPHPAINTER_H
