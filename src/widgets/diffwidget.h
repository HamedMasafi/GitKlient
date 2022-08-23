#ifndef DIFFWIDGET_H
#define DIFFWIDGET_H

#include "ui_diffwidget.h"
#include <QTextOption>
#include "widgetbase.h"
#include "git/gitfile.h"

class SegmentConnector;
class CodeEditor;
class DiffWidget : public WidgetBase, private Ui::DiffWIdget
{
    Q_OBJECT

    Q_PROPERTY(bool sameSize READ sameSize WRITE setSameSize NOTIFY sameSizeChanged)

    Git::File _oldFile;
    Git::File _newFile;

    QTextOption _defaultOption;
public:
    explicit DiffWidget(QWidget *parent = nullptr);
    DiffWidget(const Git::File &oldFile, const Git::File &newFile, QWidget *parent = nullptr);

    const Git::File &oldFile() const;
    void setOldFile(const Git::File &newOldFile);
    const Git::File &newFile() const;
    void setNewFile(const Git::File &newNewFile);

    void compare();

    CodeEditor *oldCodeEditor() const;

    CodeEditor *newCodeEditor() const;

    bool sameSize() const;
    void setSameSize(bool newSameSize);

    void scrollToTop();
    
    void setOldFileText(const Git::File &newOldFile);
    
    void setNewFileText(const Git::File &newNewFile);
    
public slots:
    void showHiddenChars(bool show);
    void showFilesInfo(bool show);
    void showSameSize(bool show);

signals:
    void sameSizeChanged();

private slots:
    void on_splitter_splitterMoved(int, int);
    void oldCodeEditor_scroll(int value);
    void newCodeEditor_scroll(int value);
    void oldCodeEditor_blockSelected();
    void newCodeEditor_blockSelected();

private:
    bool m_sameSize{false};

    void recalculateInfoPaneSize();

protected:
    void resizeEvent(QResizeEvent *event) override;
    void showEvent(QShowEvent *event) override;
};

#endif // DIFFWIDGET_H
