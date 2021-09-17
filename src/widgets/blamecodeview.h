#ifndef BLAMECODEVIEW_H
#define BLAMECODEVIEW_H

#include "codeeditor.h"
#include "git/blamedata.h"

class BlameCodeView : public CodeEditor
{
    Q_OBJECT

    Git::BlameData _blameData;

public:
    BlameCodeView(QWidget *parent = nullptr);
    const Git::BlameData &blameData() const;
    void setBlameData(const Git::BlameData &newBlameData);

    // CodeEditor interface
private:
    int sidebarWidth() const override;
    void sidebarPaintEvent(QPaintEvent *event) override;
};

#endif // BLAMECODEVIEW_H
