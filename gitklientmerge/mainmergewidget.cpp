#include "mainmergewidget.h"

#include <QTextEdit>
#include <QTextDocument>
#include <QTextBlock>

//void compare(QTextEdit *e1, QTextEdit *e2)
//{
//    auto m = qMin(e1->document()->blockCount(), e2->document()->blockCount());
//    for (int i = 0; i < m; ++i) {
//        auto block1 = e1->document()->findBlock(i);
//        auto block2 = e2->document()->findBlock(i);
//        if (block1.text() != block2.text()) {
//            //            block1.blockFormat()
//        }
//    }
//}

//QStringList readFile(const QString &filePath)
//{
//    QFile f{filePath};
//    if (!f.open(QIODevice::ReadOnly))
//        return QStringList();
//    auto buf = QString(f.readAll()).split('\n');
//    f.close();
//    return buf;
//}

MainMergeWidget::MainMergeWidget(QWidget *parent) :
      QWidget(parent)
{
    setupUi(this);
}
