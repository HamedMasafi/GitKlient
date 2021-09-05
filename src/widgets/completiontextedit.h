#ifndef COMPLETIONTEXTEDIT_H
#define COMPLETIONTEXTEDIT_H

#include <QTextEdit>

class QCompleter;
class QStringListModel;
class CompletionTextEdit : public QTextEdit
{
    Q_OBJECT

public:
    CompletionTextEdit(QWidget *parent = nullptr);

    void setCompleter(QCompleter *c);
    QCompleter *completer() const;

    void addWord(const QString &word);
    void addWords(const QStringList &words);

    const QStringList &words() const;
    void setWords(const QStringList &newWords);

    void begin();

protected:
    void keyPressEvent(QKeyEvent *e) override;
    void focusInEvent(QFocusEvent *e) override;

private Q_SLOTS:
    void insertCompletion(const QString &completion);

private:
    QString textUnderCursor() const;

private:
    QStringList _words;
    QStringListModel *_completionModel;
    QCompleter *c{nullptr};
};

#endif // COMPLETIONTEXTEDIT_H
