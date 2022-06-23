#include "../../src/diff/diff.cpp"
#include "../common/gittestmanager.cpp"
#include <QtTest/QTest>
#include <QDebug>
#include <QTextStream>

class DiffTest : public QObject
{
    Q_OBJECT

        void diff2();
    void merge();

    void mergeSqlModel();
    void array();
private Q_SLOTS:
    void diff();
};


auto segmentTypeText(Diff::SegmentType type) -> QString
{
    switch (type) {
    case Diff::SegmentType::OnlyOnRight:
        return "Added";
    case Diff::SegmentType::OnlyOnLeft:
        return "Removed";
    case Diff::SegmentType::SameOnBoth:
        return "Unchanged";
    case Diff::SegmentType::DifferentOnBoth:
        return "Modified";
    default:
        return "Unknown";
    }
}

void print(Diff::Segment *segment)
{
    qDebug().noquote() << "==================";
    qDebug().noquote() << "Segment" << segmentTypeText(segment->type);
    qDebug() << "Old:";
    qDebug().noquote() << segment->oldText;
    qDebug() << "New:";
    qDebug().noquote() << segment->newText;
}

void print(Diff::MergeSegment *segment)
{
    qDebug().noquote() << "==================";
    qDebug().noquote() << "* Segment" << segmentTypeText(segment->type);
    qDebug().noquote() << "  Base:" << segment->base;
    qDebug().noquote() << "  Local:" << segment->local;
    qDebug().noquote() << "  Remote:" << segment->remote;
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

//    for (auto &s: segments)
//        print(s);

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
6
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

    for (auto &s: segments)
        print(s);

    //    qDebug() <<lcs .size();
//        print(segments.at(0));
//    print(segments.at(1));
//    print(segments.at(2));

//    QCOMPARE(segments.size(), 3);
//    QCOMPARE(segments.at(0)->type, Diff::SegmentType::SameOnBoth);
//    QCOMPARE(segments.at(1)->type, Diff::SegmentType::DifferentOnBoth);
    //    QCOMPARE(segments.at(2)->type, Diff::SegmentType::SameOnBoth);
}

void DiffTest::mergeSqlModel()
{
    auto base = Diff::readFileLines("sqlmodel_BASE_32623.h");
    auto local= Diff::readFileLines("sqlmodel_LOCAL_32623.h");
    auto remote = Diff::readFileLines("sqlmodel_REMOTE_32623.h");

    auto segments = Diff::diff3(base, local, remote);

    for (auto &s: segments)
        print(s);
}

void DiffTest::array()
{
    Diff::Impl::Array3<int> a(5, 5, 5);
    a(1, 2, 3) = 4;
    QCOMPARE(a(1, 2, 3), 4);
}

QTEST_MAIN(DiffTest)


#include "difftest.moc"
