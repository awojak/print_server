#ifndef ICOMMANDINTERPRETER_H
#define ICOMMANDINTERPRETER_H

#include <types.h>

class ICommandInterpreter
{
public:
    ICommandInterpreter();
    virtual ~ICommandInterpreter();
    virtual bool commandProceed(uint8_t min_id, uint8_t *min_payload, uint8_t len_payload) = 0;
};

#endif // ICOMMANDINTERPRETER_H
