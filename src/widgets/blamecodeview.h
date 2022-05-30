// Copyright (C) 2020 Hamed Masafi <hamed.masafi@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef BLAMECODEVIEW_H
#define BLAMECODEVIEW_H

#include "codeeditor.h"
#include "git/blamedata.h"

#include <QMap>

class BlameCodeView : public CodeEditor
{
    Q_OBJECT

    Git::BlameData _blameData;
    QMap<int, Git::BlameDataRow> _blames;

public:
    BlameCodeView(QWidget *parent = nullptr);
    const Git::BlameData &blameData() const;
    void setBlameData(const Git::BlameData &newBlameData);

    Git::BlameDataRow blameData(const int &blockNumber) const;
    // CodeEditor interface
private:
    int sidebarWidth() const override;
    void sidebarPaintEvent(QPaintEvent *event) override;
};

#endif // BLAMECODEVIEW_H
