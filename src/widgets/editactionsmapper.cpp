#include "editactionsmapper.h"

#include <KActionCollection>
#include <QEvent>
#include <QPlainTextEdit>

#include <KStandardAction>

EditActionsMapper::EditActionsMapper(QObject *parent) : QObject(parent)
{

}

void EditActionsMapper::addTextEdit(QPlainTextEdit *control)
{
    _textEdits.append(control);

    control->installEventFilter(this);

    control->setContextMenuPolicy(Qt::DefaultContextMenu);
    connect(control, &QPlainTextEdit::copyAvailable, this, &EditActionsMapper::control_copyAvailable);
    connect(control,
            &QPlainTextEdit::selectionChanged,
            this,
            &EditActionsMapper::control_selectionChanged);
    connect(control, &QPlainTextEdit::undoAvailable, this, &EditActionsMapper::control_undoAvailable);
    connect(control, &QPlainTextEdit::redoAvailable, this, &EditActionsMapper::control_redoAvailable);
}

void EditActionsMapper::init(KActionCollection *actionCollection) const
{
    KStandardAction::cut(this, &EditActionsMapper::actionCut_triggered, actionCollection);
    KStandardAction::copy(this, &EditActionsMapper::actionCopy_triggered, actionCollection);
    KStandardAction::paste(this, &EditActionsMapper::actionPaste_triggered, actionCollection);
    KStandardAction::selectAll(this, &EditActionsMapper::actionSelectAll_triggered, actionCollection);
    KStandardAction::undo(this, &EditActionsMapper::actionUndo_triggered, actionCollection);
    KStandardAction::redo(this, &EditActionsMapper::actionRedo_triggered, actionCollection);
}

void EditActionsMapper::control_undoAvailable(bool b)
{
    Q_UNUSED(b)
}

void EditActionsMapper::control_redoAvailable(bool b)
{
    Q_UNUSED(b)
}

void EditActionsMapper::control_copyAvailable(bool b)
{
    Q_UNUSED(b)
}

void EditActionsMapper::control_selectionChanged()
{

}

void EditActionsMapper::actionUndo_triggered()
{
    if (_activeControl)
        _activeControl->undo();
}

void EditActionsMapper::actionRedo_triggered()
{
    if (_activeControl)
        _activeControl->redo();
}

void EditActionsMapper::actionCopy_triggered()
{
    if (_activeControl)
        _activeControl->copy();
}

void EditActionsMapper::actionCut_triggered()
{
    if (_activeControl)
        _activeControl->cut();
}

void EditActionsMapper::actionPaste_triggered()
{
    if (_activeControl)
        _activeControl->paste();
}

void EditActionsMapper::actionSelectAll_triggered()
{
    if (_activeControl)
        _activeControl->selectAll();
}

void EditActionsMapper::actionDelete_triggered()
{
}

bool EditActionsMapper::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() != QEvent::FocusIn && event->type() != QEvent::FocusOut)
        return QObject::eventFilter(watched, event);
    auto textEdit = qobject_cast<QPlainTextEdit *>(watched);
    if (!textEdit || textEdit == _activeControl)
        return QObject::eventFilter(watched, event);
    //    auto e = static_cast<QFocusEvent*>(event);

    if (event->type() != QEvent::FocusIn)
        _activeControl = textEdit;
    else
        _activeControl = nullptr;

    return QObject::eventFilter(watched, event);
}
