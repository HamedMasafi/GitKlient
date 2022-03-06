#ifndef FILEACTIONS_H
#define FILEACTIONS_H

#include "abstractactions.h"

class FileActions : public AbstractActions
{
    Q_OBJECT

    QString _place;
    QString _filePath;
    QAction *actionSaveAs;
//public:
//    QAction *actionSaveAs();

public:
    FileActions(Git::Manager *git, QWidget *parent = nullptr);
    void popup(const QPoint &pos);

    const QString &place() const;
    void setPlace(const QString &newPlace);

    const QString &filePath() const;
    void setFilePath(const QString &newFilePath);

private slots:
    void viewFile();
    void saveAsFile();
    void logFile();
    void blameFile();
    void search();
};

#endif // FILEACTIONS_H
