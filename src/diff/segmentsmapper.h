#ifndef SEGMENTSMAPPER_H
#define SEGMENTSMAPPER_H

#include "diff/diff.h"
#include <QObject>

class CodeEditor;
class SegmentsMapper : public QObject
{
    Q_OBJECT
public:
    SegmentsMapper(QObject *parent = nullptr);

    void addEditor(CodeEditor *editor);
    const QList<Diff::MergeSegment *> &segments() const;
    void setSegments(const QList<Diff::MergeSegment *> &newSegments);

    int mapIndexFromOldToNew(int oldIndex);
    int mapIndexFromNewToOld(int newIndex);
    int map(int from, int to, int index) const;

private slots:
    void codeEditor_blockSelected();
    void codeEditor__scroll();

private:
    QList<Diff::MergeSegment *> _segments;
    QList<CodeEditor *> _editors;
};

#endif // SEGMENTSMAPPER_H
