#ifndef COMMANDINTERPRETER_H
#define COMMANDINTERPRETER_H

#include "icommandinterpreter.h"

class CommandInterpreter : public ICommandInterpreter
{
public:
    CommandInterpreter();
    virtual bool commandProceed(uint8_t min_id, uint8_t *min_payload, uint8_t len_payload);
};

#endif // COMMANDINTERPRETER_H
