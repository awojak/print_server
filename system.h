#ifndef SYSTEM_H
#define SYSTEM_H

#include "isystem.h"

class System : public ISystem
{
public:
    System();
    ~System();
    virtual int getCurrentTimeInMs();

};

#endif // SYSTEM_H
