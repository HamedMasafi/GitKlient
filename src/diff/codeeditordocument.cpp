#include "codeeditordocument.h"

#include <QPlainTextDocumentLayout>
#include <QTextFrame>

CodeEditorDocument::CodeEditorDocument(QObject *parent) : QTextDocument(parent)
{
    auto stylesheet = QStringLiteral(R"~(
body {
    margin-left: 30px;
    padding-left: 40px;
    font-size: 30pt;
}
p {
    background-color: red;
    font-size: 30pt;
}

)~");
    setDefaultStyleSheet(stylesheet);
    setDocumentLayout(new QPlainTextDocumentLayout(this));
    QTextOption opt;
    opt.setFlags(opt.flags() | QTextOption::IncludeTrailingSpaces | QTextOption::ShowLineAndParagraphSeparators);
//    setDefaultTextOption(opt);
}

QTextObject *CodeEditorDocument::createObject(const QTextFormat &f)
{
    Q_UNUSED(f)
    auto frame = new QTextFrame(this);
    return frame;
}
