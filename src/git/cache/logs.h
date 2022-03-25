//
// Created by hamed on 25.03.22.
//

#ifndef GITKLIENT_LOGS_H
#define GITKLIENT_LOGS_H

#include "git/cache/cache.h"
#include <QAbstractListModel>

namespace Git {

    class LogsCache : public Cache, public QAbstractListModel {

    };

}

#endif //GITKLIENT_LOGS_H
