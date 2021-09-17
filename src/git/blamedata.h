#ifndef BLAMEDATA_H
#define BLAMEDATA_H

#include <QDateTime>
#include <QString>
#include "gitloglist.h"

namespace Git {

struct BlameDataRow {
    QString commitHash;
    QString code;

    Log *log;
};

class BlameData : public QList<BlameDataRow>
{
public:
    BlameData();

    void initCommits(const LogList &logs);
};

} // namespace Git

#endif // BLAMEDATA_H
