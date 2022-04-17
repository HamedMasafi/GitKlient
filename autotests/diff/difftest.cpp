#include "../../src/diff/diff.cpp"
#include <QtTest/QTest>
#include <QDebug>
#include <QTextStream>

class DiffTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void diff();
    void diff2();
    void files();
    void merge();
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
//    qDebug().noquote() << "==================";
//    qDebug().noquote() << "Segment" << typeString;
//    qDebug() << "Old:";
//    qDebug().noquote() << segment->oldText;
//    qDebug() << "New:";
//    qDebug().noquote() << segment->newText;
//    qDebug() << Qt::flush;
}

void DiffTest::diff()
{
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
    QCOMPARE(segments.at(0)->type, Diff::SegmentType::SameOnBoth);
    QCOMPARE(segments.at(1)->type, Diff::SegmentType::OnlyOnRight);
    QCOMPARE(segments.at(2)->type, Diff::SegmentType::SameOnBoth);
    QCOMPARE(segments.at(3)->type, Diff::SegmentType::DifferentOnBoth);
    QCOMPARE(segments.at(4)->type, Diff::SegmentType::SameOnBoth);
}

void DiffTest::diff2()
{
    QString oldCode{R"~(
1
2
4
7
8
)~"};

    QString newCode{R"~(
1
3
4
5
6
8
)~"};

    auto lcs = Diff::Impl::longestCommonSubsequence(oldCode.split('\n'),
                                                    newCode.split('\n'));

    qDebug() << "**" << lcs.size();

    auto segments = Diff::diff(oldCode.split("\n"), newCode.split("\n"));
    QCOMPARE(segments.size(), 5);
    QCOMPARE(segments.at(0)->type, Diff::SegmentType::SameOnBoth);
    QCOMPARE(segments.at(1)->type, Diff::SegmentType::DifferentOnBoth);
    QCOMPARE(segments.at(2)->type, Diff::SegmentType::SameOnBoth);
    QCOMPARE(segments.at(3)->type, Diff::SegmentType::DifferentOnBoth);
    QCOMPARE(segments.at(4)->type, Diff::SegmentType::SameOnBoth);
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

void DiffTest::merge()
{
    QString base{R"~(
one
two
three
)~"};

    QString mine{R"~(
one
four
)~"};

    QString their{R"~(
one
two
five
)~"};

    auto lcs = Diff::Impl::longestCommonSubsequence(base.split('\n'),
                                                    mine.split('\n'),
                                                    their.split('\n'));
    QCOMPARE(lcs.size(), 3);

    auto segments = Diff::diff3(base, mine, their);

    //    qDebug() <<lcs .size();
    //    print(segments.at(0));
    QCOMPARE(segments.size(), 3);
    //    QCOMPARE(segments.at(0)->type, Diff::SegmentType::SameOnBoth);
    //    QCOMPARE(segments.at(1)->type, Diff::SegmentType::DifferentOnBoth);

}

QTEST_MAIN(DiffTest)


#include "difftest.moc"
