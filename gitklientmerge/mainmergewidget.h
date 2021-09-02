#ifndef MAINMERGEWIDGET_H
#define MAINMERGEWIDGET_H

#include "ui_mainmergewidget.h"

class MainMergeWidget : public QWidget, private Ui::MainMergeWidget
{
    Q_OBJECT

public:
    explicit MainMergeWidget(QWidget *parent = nullptr);
};

#endif // MAINMERGEWIDGET_H
