#ifndef FILEACTIONS_H
#define FILEACTIONS_H

#include <QObject>


namespace Git {
class Manager;
};

class QWidget;
class QMenu;
class QAction;
class FileActions : public QObject
{
    Q_OBJECT

    Git::Manager *_git;
    QMenu *_fileMenu;
    QWidget *_parent;
    QString _place;
    QString _filePath;
    QAction *actionSaveAs;
//public:
//    QAction *actionSaveAs();

public:
    FileActions(Git::Manager *git, QWidget *parent = nullptr);

private slots:
    void viewFile();
    void saveAsFile();
    void logFile();
    void blameFile();
    void search();
};

#endif // FILEACTIONS_H
