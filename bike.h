#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>

#define PIN_INPUT   0
#define PIN_OUTPUT  1
#define PULL_DISABLED 0
#define PULL_ENABLED  1

uint32_t ONGOING_TIMER;
uint32_t CURRENT_TIMER;
uint32_t START_TIMER;

//WHIMSICAL FUNCTIONS!
void timer0_init(); //Use Timer0
void millis(); //timer function in milliseconds

void bike_init(void);
uint8_t button_pressed(void);
uint16_t ultrasonic_read(uint8_t trig_id, uint8_t echo_id);
void bike_update(void);
