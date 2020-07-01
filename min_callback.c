#include "min.h"
#include "types.h"

// CALLBACK. Handle incoming MIN frame
void min_application_handler(uint8_t min_id, uint8_t *min_payload, uint8_t len_payload, uint8_t port)
{

}

#ifdef TRANSPORT_PROTOCOL
// CALLBACK. Must return current time in milliseconds.
// Typically a tick timer interrupt will increment a 32-bit variable every 1ms (e.g. SysTick on Cortex M ARM devices).
uint32_t min_time_ms(void)
{
    //@TODO
    return 0;
}
#endif

// CALLBACK. Must return current buffer space in the given port. Used to check that a frame can be
// queued.
uint16_t min_tx_space(uint8_t port)
{
    //@TODO
    return 0;
}

// CALLBACK. Send a byte on the given line.
void min_tx_byte(uint8_t port, uint8_t byte)
{

}

// CALLBACK. Indcates when frame transmission is finished; useful for buffering bytes into a single serial call.
void min_tx_start(uint8_t port)
{

}
void min_tx_finished(uint8_t port)
{

}
