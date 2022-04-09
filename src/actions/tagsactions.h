#ifndef TAGSACTIONS_H
#define TAGSACTIONS_H

#include "abstractactions.h"

class TagsActions : public AbstractActions
{
    Q_OBJECT

    DEFINE_ACTION(actionCreate)
    DEFINE_ACTION(actionRemove)
    DEFINE_ACTION(actionCheckout)

public:
    TagsActions(Git::Manager *git, QWidget *parent = nullptr);

    const QString &tagName() const;
    void setTagName(const QString &newTagName);

private slots:
    void create();
    void remove();
    void checkout();

private:
    QString _tagName;
};

#endif // TAGSACTIONS_H
