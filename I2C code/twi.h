
#include <avr/io.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <util/twi.h>
#include <avr/interrupt.h>

#define BUFFER buffer_stuff.buffer_info.buffer           // Calling the buffer itself and assigning it to a define macro for ease of use
#define BUFFER_SIZE buffer_stuff.buffer_info.buffer_size // doing the same thing but for the buffer count respectively

typedef enum
{ // Part 2
    twi_status_success,
    twi_status_failure,
    twi_status_busy
} twi_status_t;

typedef struct
{ // Part 3
    uint8_t address;
    volatile uint8_t *buffer;
    uint8_t buffer_size;
} twi_message_t;

twi_status_t twi_init(const uint32_t set_frequency);
twi_status_t twi_enqueue(twi_message_t *messages, size_t count);
twi_status_t twi_status(void);
twi_status_t twi_cancel(void);
twi_status_t twi_deinit(void);

// Part 6
static struct ISR_TO_FUNC
{                              // I created this struc that way I can access the buffer_info variable from the twi_message_t data type. It's also static that way the buffer info remains secure.
    twi_message_t buffer_info; // Linking the information from the twi_message_t datatype to one variable
} buffer_info;