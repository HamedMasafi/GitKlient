#ifndef CODEEDITORDOCUMENT_H
#define CODEEDITORDOCUMENT_H

#include <QTextDocument>

class CodeEditorDocument : public QTextDocument
{
    Q_OBJECT

public:
    explicit CodeEditorDocument(QObject *parent = nullptr);

protected:
    QTextObject *createObject(const QTextFormat &f) override;
};

#endif // CODEEDITORDOCUMENT_H
