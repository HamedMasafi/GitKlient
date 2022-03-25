//
// Created by hamed on 25.03.22.
//

#ifndef GITKLIENT_CACHE_H
#define GITKLIENT_CACHE_H

#include <QObject>

namespace Git {

    class Cache : public QObject {
        Q_OBJECT

    public:
        bool isLoaded() const;

    protected:
        void setLoaded(bool loaded);

    signals:
        void loaded();

    private:
        bool _isLoaded{false};
    };
}

#endif //GITKLIENT_CACHE_H
