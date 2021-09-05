#ifndef DIFFWIDGET_H
#define DIFFWIDGET_H

//#include "ui_diffwidget.h"
#include <QTextOption>
#include "widgetbase.h"
#include "git/gitfile.h"

class SegmentConnector;
class CodeEditor;
class DiffWidget : public WidgetBase//, private Ui::DiffWIdget
{
    Q_OBJECT

    Q_PROPERTY(bool sameSize READ sameSize WRITE setSameSize NOTIFY sameSizeChanged)

    Git::File _oldFile;
    Git::File _newFile;

    CodeEditor *_oldCodeEditor;
    CodeEditor *_newCodeEditor;
    SegmentConnector *_segmentConnector;

    QTextOption _defaultOption;
public:
    DiffWidget(QWidget *parent = nullptr);
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

public slots:
    void showHiddenChars(bool show);

signals:
    void sameSizeChanged();

private slots:

    void oldCodeEditor_scroll(int value);
    void newCodeEditor_scroll(int value);
    void oldCodeEditor_blockSelected();
    void newCodeEditor_blockSelected();
private:
    void setupUi();

    bool m_sameSize{false};
};

#endif // DIFFWIDGET_H
