#include "../../core/diff/diff.h"
#include <QtTest/QTest>
#include <QDebug>


class DiffTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void diff();
    void files();

};

void print(Diff::Segment *segment)
{
    QString typeString;
    switch (segment->type) {
    case Diff::SegmentType::OnlyOnRight:
        typeString = "Added";
        break;
    case Diff::SegmentType::OnlyOnLeft:
        typeString = "Removed";
        break;
    case Diff::SegmentType::SameOnBoth:
        typeString = "Unchanged";
        break;
    case Diff::SegmentType::DifferentOnBoth:
        typeString = "Modified";
        break;
    }
    qDebug().noquote() << "==================";
    qDebug().noquote() << "Segment" << typeString;
    qDebug() << "Old:";
    qDebug().noquote() << segment->oldText;
    qDebug() << "New:";
    qDebug().noquote() << segment->newText;
    qDebug() << Qt::flush;
}

void DiffTest::diff()
{
    return;
    QString oldCode{R"~(
#include <iostream>
int main()
{
    int n = 3;
    std::cout << n;
}
)~"};

    QString newCode{R"~(
#include <iostream>
//new comment
int main()
{
    int m = 3;
    std::cout << m;
}
)~"};

    auto segments = Diff::diff(oldCode.split("\n"), newCode.split("\n"));

    for (auto &s: segments)
        print(s);

    QCOMPARE(segments.size(), 5);
//    QCOMPARE(segments.at(0)->type, Diff::DiffType::Unchanged);
//    QCOMPARE(segments.at(1)->type, Diff::DiffType::Added);
//    QCOMPARE(segments.at(2)->type, Diff::DiffType::Unchanged);
//    QCOMPARE(segments.at(3)->type, Diff::DiffType::Modified);
    //    QCOMPARE(segments.at(4)->type, Diff::DiffType::Unchanged);
}

QString diffTypeText(const Diff::DiffType type)
{
    switch (type) {
    case Diff::DiffType::Unchanged: return "Unchanged";
    case Diff::DiffType::Added: return "Added";
    case Diff::DiffType::Removed: return "Removed";
    case Diff::DiffType::Modified: return "Modified";
    }
    return QString();
}

void DiffTest::files()
{
    auto map = Diff::diffDirs("/home/hamed/tmp/diff/1", "/home/hamed/tmp/diff/2");
    for (auto i = map.begin(); i != map.end(); ++i) {
        qDebug() << i.key() << diffTypeText(i.value());
    }
}

QTEST_MAIN(DiffTest)


#include "difftest.moc"
