//
// Created by hamed on 25.03.22.
//

#ifndef GITKLIENT_CACHE_H
#define GITKLIENT_CACHE_H

#include <QAbstractListModel>

namespace Git {

class Manager;
class AbstractGitItemsModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(Status status READ status WRITE setStatus NOTIFY statusChanged)

public:
    explicit AbstractGitItemsModel(Manager *git, QObject *parent = nullptr);

    enum Status { NotLoaded, Loading, Loaded };
    Q_ENUM(Status)
    bool isLoaded() const;
    Status status() const;

public slots:
    void load();

protected:
    void setStatus(Status newStatus);
    Manager *_git{nullptr};
    virtual void fill() = 0;

signals:
    void loaded();
    void statusChanged();

private:
    Status m_status{NotLoaded};
};
} // namespace Git

#endif //GITKLIENT_CACHE_H
