#ifndef CODEEDITORSIDEBAR_H
#define CODEEDITORSIDEBAR_H

#include <QWidget>

class CodeEditor;
class CodeEditorSidebar : public QWidget
{
    Q_OBJECT
public:
    explicit CodeEditorSidebar(CodeEditor *editor);
    QSize sizeHint() const override;

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    CodeEditor *m_codeEditor;
};

#endif // CODEEDITORSIDEBAR_H
