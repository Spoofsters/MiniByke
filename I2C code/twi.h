
#include <avr/io.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <util/twi.h>

typedef enum{ // Part 2
    twi_status_success,
    twi_status_failure,
    twi_status_busy
} twi_status_t;


typedef struct { // Part 3
    volatile uint8_t * buffer;
    uint8_t buffer_size;
} twi_message_t;

twi_status_t twi_init(const uint32_t set_frequency);
twi_status_t twi_enqueue(twi_message_t *messages, size_t count);
twi_status_t twi_status(void);
twi_status_t twi_cancel(void);
twi_status_t twi_deinit(void);

