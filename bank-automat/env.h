#ifndef ENV_H
#define ENV_H

#include <qstring.h>

class env
{
public:
    env();

    static QString getUrl();
    static QString gifFetchArrows();
    static QString gifFetchMoney();
};

#endif // ENV_H
