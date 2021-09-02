#include "multipagewidget.h"
#include "widgetbase.h"

#include <QAction>
#include <QToolButton>

Git::Manager *MultiPageWidget::defaultGitManager() const
{
    return _defaultGitManager;
}

void MultiPageWidget::setDefaultGitManager(Git::Manager *newDefaultGitManager)
{
    _defaultGitManager = newDefaultGitManager;
}

int MultiPageWidget::count() const
{
    return actionGroup->actions().size();
}

MultiPageWidget::MultiPageWidget(QWidget *parent) :
      QWidget(parent)
{
    actionGroup = new QActionGroup(this);
    Q_SET_OBJECT_NAME(actionGroup);

    setupUi(this);

    auto styleSheet = QString(R"CSS(
        #scrollAreaWidgetContents {
            background-color: #%1;
        }
        QToolButton {
            background-color: #%1;
            border: none;
            padding-top: 10px;
            padding-bottom: 10px;
            height: 48px;
        }

        QToolButton:hover {
            background-color: #%2;
        }

        QToolButton:checked {
            background-color: #%3;
            color: white;
        }

)CSS")
                          .arg(palette().color(QPalette::Base).rgba(), 0, 16)
                          .arg(palette().color(QPalette::Highlight).lighter().rgba(), 0, 16)
                          .arg(palette().color(QPalette::Highlight).rgba(), 0, 16);

    scrollAreaWidgetContents->setStyleSheet(styleSheet);
}

void MultiPageWidget::addPage(const QString &title, const QIcon &icon, WidgetBase *widget)
{
    const QList<Qt::Key> keys = {
        Qt::Key_0,
        Qt::Key_1,
        Qt::Key_2,
        Qt::Key_3,
        Qt::Key_4,
        Qt::Key_5,
        Qt::Key_6,
        Qt::Key_7,
        Qt::Key_8,
        Qt::Key_9
    };
    auto btn = new QToolButton(this);
    btn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    btn->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed));

    auto action = new QAction(this);
    action->setText(title);
    action->setIcon(icon);
    action->setCheckable(true);
    action->setData(actionGroup->actions().size());
    if (actionGroup->actions().size() <10)
        action->setShortcut(QKeySequence(Qt::CTRL + keys[actionGroup->actions().size()]));
    btn->setDefaultAction(action);
    actionGroup->addAction(action);

    stackedWidget->addWidget(widget);

    widget->layout()->setMargin(0);

    verticalLayoutButtons->insertWidget(actionGroup->actions().size() - 1, btn);
}

void MultiPageWidget::addPage(WidgetBase *widget, QAction *action)
{
    auto btn = new QToolButton(this);
    btn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    btn->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed));

    if (!action)
        action = new QAction(this);
    action->setText(widget->windowTitle());
    action->setIcon(widget->windowIcon());
    action->setCheckable(true);
    action->setData(actionGroup->actions().size());
    btn->setDefaultAction(action);
    actionGroup->addAction(action);

    stackedWidget->addWidget(widget);

    widget->layout()->setMargin(0);

    verticalLayoutButtons->insertWidget(actionGroup->actions().size() - 1, btn);
}

void MultiPageWidget::setCurrentIndex(int index)
{
    actionGroup->actions().at(index)->trigger();
}

QList<QAction *> MultiPageWidget::actions() const
{
    return actionGroup->actions();
}

void MultiPageWidget::on_actionGroup_triggered(QAction *action)
{
    stackedWidget->setCurrentIndex(action->data().toInt());
    labelTitle->setText(action->text().replace("&", ""));
    labelPageIcon->setPixmap(action->icon().pixmap({32, 32}));
}
