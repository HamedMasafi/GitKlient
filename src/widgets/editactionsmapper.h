#ifndef EDITACTIONSMAPPER_H
#define EDITACTIONSMAPPER_H

#include <QObject>

class QPlainTextEdit;
class QAction;
class KActionCollection;
class EditActionsMapper : public QObject
{
    Q_OBJECT

    QList<QPlainTextEdit*> _textEdits;
    QPlainTextEdit *_activeControl{nullptr};

    QAction *_actionUndo{nullptr};
    QAction *_actionRedo{nullptr};
    QAction *_actionCopy{nullptr};
    QAction *_actionCut{nullptr};
    QAction *_actionPaste{nullptr};
    QAction *_actionSelectAll{nullptr};
    QAction *_actionDelete{nullptr};

public:
    explicit EditActionsMapper(QObject *parent = nullptr);
    ~EditActionsMapper() override = default;

    void addTextEdit(QPlainTextEdit *control);
    void init(KActionCollection *actionCollection) const;

private Q_SLOTS:
    void control_undoAvailable(bool b);
    void control_redoAvailable(bool b);
    void control_copyAvailable(bool b);
    void control_selectionChanged();

    void actionUndo_triggered();
    void actionRedo_triggered();
    void actionCopy_triggered();
    void actionCut_triggered();
    void actionPaste_triggered();
    void actionSelectAll_triggered();
    void actionDelete_triggered();

    // QObject interface
public:
    bool eventFilter(QObject *watched, QEvent *event) override;
};

#endif // EDITACTIONSMAPPER_H
