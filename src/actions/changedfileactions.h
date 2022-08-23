#ifndef CHANGEDFILEACTIONS_H
#define CHANGEDFILEACTIONS_H

#include "abstractactions.h"

class ChangedFileActions : public AbstractActions
{
    Q_OBJECT

    QString _filePath;

    DEFINE_ACTION(actionDiff)
    DEFINE_ACTION(actionRevert)

public:
    explicit ChangedFileActions(Git::Manager *git, QWidget *parent = nullptr);

    const QString &filePath() const;
    void setFilePath(const QString &newFilePath);

public slots:
    void diff();
    void revert();

signals:
    void reloadNeeded();
};

#endif // CHANGEDFILEACTIONS_H
