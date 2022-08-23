#ifndef GITFILE_H
#define GITFILE_H

#include <QString>

namespace Git {

class Manager;
class File
{
    QString _place;
    QString _filePath;
    Manager *_git{};

    enum StorageType {
        InValid,
        Git,
        Local
    };
    StorageType _storage;

public:
    File();
    explicit File(QString filePath);
    File(QString place, QString filePath, Manager *git = nullptr);
    File(const File &other);
//    File(File &&other);

    File &operator=(const File &other);

    bool save(const QString &path) const;
    QString content() const;
    const QString &place() const;
    void setPlace(const QString &newPlace);
    const QString &fileName() const;
    void setFileName(const QString &newFileName);
    Manager *git() const;
    void setGit(Manager *newGit);

    QString displayName() const;
    StorageType storage() const;
};

} // namespace Git

#endif // GITFILE_H
