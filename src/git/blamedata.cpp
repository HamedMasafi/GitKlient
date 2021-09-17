#include "blamedata.h"

#include <QDebug>

namespace Git {

BlameData::BlameData()
{

}

void BlameData::initCommits(const LogList &logs)
{
//    for (auto &l: *this) {
//        l.log = logs.findByHash(l.commitHash);
//        if (!l.log ) {
//            qDebug() << "No log found";
//        }
//    }
}

}
