#ifndef HISTORYVIEW_H
#define HISTORYVIEW_H

#include <QTreeView>

class HistoryView : public QTreeView
{
    Q_OBJECT

public:
    explicit HistoryView(QWidget *parent = nullptr);
};

#endif // HISTORYVIEW_H
