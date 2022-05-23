#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <KSyntaxHighlighting/Repository>
#include <QMap>
#include <QPlainTextEdit>
#include <QTextBlockFormat>
#include <diff/diff.h>

namespace KSyntaxHighlighting
{
class SyntaxHighlighter;
}

class CodeEditorSidebar;

class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT
    QMap<int, Diff::Segment*> _segments;
    QPair<int, int> _currentSegment{-1, -1};

public:
    enum BlockType {
        Unchanged,
        Added,
        Removed,
        Edited,
        HighLight,
        Empty
    };

    explicit CodeEditor(QWidget *parent = nullptr);
    ~CodeEditor();

    void setHighlighting(const QString &fileName);

    void append(const QString &code, const BlockType &type = Unchanged, Diff::Segment *segment = nullptr, bool isEmpty = false);
    int append(const QString &code, const QColor &backgroundColor);
    void append(const QStringList &code, const BlockType &type = Unchanged, Diff::Segment *segment = nullptr, int size = -1);

    QPair<int, int> blockArea(int from, int to);

    int currentLineNumber() const;
    void gotoLineNumber(int lineNumber);
    void gotoSegment(Diff::Segment *segment);

    Diff::Segment *currentSegment() const;
    void highlightSegment(Diff::Segment *segment);

    void clearAll();

Q_SIGNALS:
    void blockSelected();

protected:
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    virtual int sidebarWidth() const;
    virtual void sidebarPaintEvent(QPaintEvent *event);
    KSyntaxHighlighting::SyntaxHighlighter *m_highlighter;
    CodeEditorSidebar *m_sideBar;

private:
    friend class CodeEditorSidebar;
    QMap<BlockType, QTextBlockFormat> _formats;
    void setTheme(const KSyntaxHighlighting::Theme &theme);

    void updateSidebarGeometry();
    void updateSidebarArea(const QRect &rect, int dy);
    void highlightCurrentLine();

    QTextBlock blockAtPosition(int y) const;
    bool isFoldable(const QTextBlock &block) const;
    bool isFolded(const QTextBlock &block) const;
    void toggleFold(const QTextBlock &block);

    KSyntaxHighlighting::Repository m_repository;


protected:
    void paintEvent(QPaintEvent *e) override;

private:
    QMap<QTextBlock, BlockType> _lines;
};

#endif // CODEEDITOR_H
