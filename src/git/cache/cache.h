//
// Created by hamed on 25.03.22.
//

#ifndef GITKLIENT_CACHE_H
#define GITKLIENT_CACHE_H

#include <QObject>

namespace Git {

class Manager;
class Cache : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Status status READ status WRITE setStatus NOTIFY statusChanged)

public:
    Cache(Manager *git);
    enum Status { NotLoaded, Loading, Loaded };
    Q_ENUM(Status)
    bool isLoaded() const;
    Status status() const;


protected:
    void setStatus(Status newStatus);

signals:
    void loaded();
    void statusChanged();

private:
    Status m_status{NotLoaded};
    Manager *_git{nullptr};
};
} // namespace Git

#endif //GITKLIENT_CACHE_H
