#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stddef.h>
#include <stdarg.h>
#include <assert.h>
#include <stdint.h>
#include <util/twi.h>
#include <avr/io.h>
#include <util/delay.h>

#define PIN_INPUT   0
#define PIN_OUTPUT  1
#define PULL_DISABLED 0
#define PULL_ENABLED  1


uint8_t ONGOING_TIMER;
uint8_t CURRENT_TIMER;
uint8_t START_TIMER;

//WHIMSICAL FUNCTIONS!
void timer0_init(); //Use Timer0
void millis(); //timer function in milliseconds

//PIN ID'S to monitor #
typedef enum {
    PIN_BUTTON,
    PIN_1_TRIG,
    PIN_1_ECHO,
    PIN_2_TRIG,
    PIN_2_ECHO,
    PIN_COUNT //6 Total
} pin_id_t;

//STRUCT FOR PINS LAYOUT
typedef struct {
    volatile uin8_t *ddr;
    volatile uin8_t *port;
    volatile uin8_t *pin_reg;
    uint8_t bit;
    uint8_t dir; //Indicator if pin is input or output
    uint8_t pull; //Indicates if pull up resistor enabled or not
} pin_def_t;

void bike_init(void);
uint8_t button_pressed(void);
uint16_t ultrasonic_read(uint8_t trig_id, uint8_t echo_id);