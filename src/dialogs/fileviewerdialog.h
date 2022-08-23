#ifndef FILEVIEWERDIALOG_H
#define FILEVIEWERDIALOG_H

#include "../core/appdialog.h"
#include "ui_fileviewerdialog.h"

#include <QPointer>

#include <KParts/MainWindow>
#include <KParts/ReadOnlyPart>
#include <KService>

class QMimeType;
namespace Git {
class Manager;
class File;
}
class FileViewerDialog : public KParts::MainWindow, private Ui::FileViewerDialog
{
    Q_OBJECT
    Git::Manager *_git;
    QString _filePath;

public:
    explicit FileViewerDialog(const QString &place, const QString &fileName, QWidget *parent = nullptr);
    explicit FileViewerDialog(Git::Manager *git, const Git::File &file, QWidget *parent = nullptr);
    ~FileViewerDialog();

private:
    void showFile(const Git::File &file);
    void showInEditor(const Git::File &file);
    void showAsImage(const Git::File &file);
    QPointer<KParts::ReadOnlyPart> m_part;

    bool viewInInternalViewer(const KService::Ptr& viewer, const QString &fileName, const QMimeType &mimeType);
    KService::Ptr getExternalViewer(const QString &mimeType);
    KService::Ptr getInternalViewer(const QString &mimeType);
};

#endif // FILEVIEWERDIALOG_H
