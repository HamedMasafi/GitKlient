#ifndef DIFFWINDOW_H
#define DIFFWINDOW_H

#include <KXmlGuiWindow>
#include <git/gitfile.h>
#include <QObject>

class DiffTreeModel;
class DiffWidget;
class QTreeView;
class DiffWindow : public KXmlGuiWindow
{
    Q_OBJECT

    Git::File _oldFile;
    Git::File _newFile;

    QString _oldBranch;
    QString _newBranch;

    DiffTreeModel *_diffModel;
    DiffWidget *_diffWidget;
    QTreeView *_treeView;

    void initActions();
    void init();

public:
    explicit DiffWindow();
    DiffWindow(const Git::File &oldFile, const Git::File &newFile);
    DiffWindow(const QString &oldBranch, const QString &newBranch);

private slots:
    void fileOpen();
};

#endif // DIFFWINDOW_H
