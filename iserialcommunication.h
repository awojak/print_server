#ifndef ISERIALCOMMUNICATION_H
#define ISERIALCOMMUNICATION_H


class ISerialCommunication
{
public:
    ISerialCommunication();
    virtual ~ISerialCommunication();

    virtual void sendByte(char c) = 0;
    virtual int transmitSpace() = 0;
};

#endif // ISERIALCOMMUNICATION_H
