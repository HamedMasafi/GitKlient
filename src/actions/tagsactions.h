#ifndef TAGSACTIONS_H
#define TAGSACTIONS_H

#include "abstractactions.h"

class TagsActions : public AbstractActions
{
    Q_OBJECT

    DEFINE_ACTION(actionCreate)
    DEFINE_ACTION(actionRemove)
    DEFINE_ACTION(actionCheckout)
    DEFINE_ACTION(actionDiff)
    DEFINE_ACTION(actionPush)

public:
    explicit TagsActions(Git::Manager *git, QWidget *parent = nullptr);

    const QString &tagName() const;
    void setTagName(const QString &newTagName);

private slots:
    void create();
    void remove();
    void checkout();
    void diff();
    void push();

private:
    QString _tagName;
};

#endif // TAGSACTIONS_H
