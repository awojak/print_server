#ifndef ISYSTEM_H
#define ISYSTEM_H


class ISystem
{
public:
    ISystem();
    virtual ~ISystem();

    virtual int getCurrentTimeInMs() = 0;
};

#endif // ISYSTEM_H
