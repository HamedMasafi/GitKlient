#include "codeeditor.h"
#include "codeeditorsidebar.h"

#include <QTextBlock>

CodeEditorSidebar::CodeEditorSidebar(CodeEditor *editor) : QWidget(editor), m_codeEditor(editor) {

    setAttribute(Qt::WA_OpaquePaintEvent);
}

QSize CodeEditorSidebar::sizeHint() const
{
    return QSize(m_codeEditor->sidebarWidth(), 0);
}

void CodeEditorSidebar::paintEvent(QPaintEvent *event)
{
    m_codeEditor->sidebarPaintEvent(event);
}

void CodeEditorSidebar::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->x() >= width() - m_codeEditor->fontMetrics().lineSpacing()) {
        auto block = m_codeEditor->blockAtPosition(event->y());
        if (!block.isValid() || !m_codeEditor->isFoldable(block))
            return;
        m_codeEditor->toggleFold(block);
    }
    QWidget::mouseReleaseEvent(event);
}
