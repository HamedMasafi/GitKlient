// Copyright (C) 2020 Hamed Masafi <hamed.masafi@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "blamecodeview.h"

#include "codeeditorsidebar.h"
#include "git/gitlog.h"

#include <KSyntaxHighlighting/Definition>
#include <KSyntaxHighlighting/FoldingRegion>
#include <KSyntaxHighlighting/SyntaxHighlighter>
#include <KSyntaxHighlighting/Theme>
#include <KLocalizedString>

#include <QPainter>
#include <QTextBlock>

BlameCodeView::BlameCodeView(QWidget *parent) : CodeEditor(parent)
{
    setReadOnly(true);
}

const Git::BlameData &BlameCodeView::blameData() const
{
    return _blameData;
}

void BlameCodeView::setBlameData(const Git::BlameData &newBlameData)
{
    QVector<QColor> colors{QColor(200, 150, 150, 100), QColor(150, 200, 150, 100)};
    int currentColor{0};
    _blameData = newBlameData;
    QString lastCommit;
    for (auto &blame: newBlameData) {
        QString commitHash = blame.log ? blame.log->commitHash() : QString();

        if (lastCommit != commitHash)
            currentColor = (currentColor + 1) % colors.size();

        auto blockNumber = append(blame.code, colors.at(currentColor));
        _blames.insert(blockNumber, blame);
        lastCommit = commitHash;
    }
}

Git::BlameDataRow BlameCodeView::blameData(const int &blockNumber) const
{
    return _blames.value(blockNumber);
}

int BlameCodeView::sidebarWidth() const
{
    int max{0};

    auto fm = fontMetrics();
    for (auto &b: _blameData) {
        QString text = b.log ? b.log->authorName() : i18n("Uncommited");

#if QT_VERSION >= QT_VERSION_CHECK(5, 11, 0)
        max = qMax(max, fm.horizontalAdvance(text));
#else
        max = qMax(max, fm.width(text));
#endif
    }

    return max + CodeEditor::sidebarWidth() + 20;
}

void BlameCodeView::sidebarPaintEvent(QPaintEvent *event)
{
    CodeEditor::sidebarPaintEvent(event);

    QPainter painter(m_sideBar);

    auto block = firstVisibleBlock();
    auto blockNumber = block.blockNumber();
    int top = blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + blockBoundingRect(block).height();
    const int currentBlockNumber = textCursor().blockNumber();

    const auto foldingMarkerSize = fontMetrics().lineSpacing();

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            if (blockNumber >= _blameData.size())
                break;
            auto &&d = _blameData.at(blockNumber);


            painter.setPen(m_highlighter->theme().editorColor(
                (blockNumber == currentBlockNumber) ? KSyntaxHighlighting::Theme::CurrentLineNumber
                                                    : KSyntaxHighlighting::Theme::LineNumbers));

            painter.drawText(0,
                             top,
                             m_sideBar->width() - 2 - foldingMarkerSize,
                             fontMetrics().height(),
                             Qt::AlignLeft,
                             d.log ? d.log->committerName() : i18n("Uncommited"));
        }

        block = block.next();
        top = bottom;
        bottom = top + blockBoundingRect(block).height();
        ++blockNumber;
    }
}
