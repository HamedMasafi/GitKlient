#ifndef FILEACTIONS_H
#define FILEACTIONS_H

#include "abstractactions.h"

#include <KService>

class FileActions : public AbstractActions
{
    Q_OBJECT

    QString _place;
    QString _filePath;

    DEFINE_ACTION(actionSaveAs)
    DEFINE_ACTION(actionView)
    DEFINE_ACTION(actionHistory)
    DEFINE_ACTION(actionBlame)
    DEFINE_ACTION(actionSearch)
    DEFINE_ACTION(actionOpenWith)
    DEFINE_ACTION(actionDiffWithHead)
    DEFINE_ACTION(actionMergeWithHead)


    QMenu *_openWithMenu;
//public:
//    QAction *actionSaveAs();

    KService::Ptr getExternalViewer(const QString &mimeType);
    KService::Ptr getViewer(const QString &mimeType);
public:
    explicit FileActions(Git::Manager *git, QWidget *parent = nullptr);
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
    void openWith();
    void diffWithHead();
    void mergeWithHead();
};

#endif // FILEACTIONS_H
