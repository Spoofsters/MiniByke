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

enum {
    L_CONST,
    H_CONST,
    TOTAL,
 } BOUNDARIES

//PIN ID'S to monitor #
typedef enum {
    PIN_BUTTON,

//ULTRASONIC LEFT
    PIN_L_TRIG,
    PIN_L_ECHO,

//ULTRASONIC RIGHT
    PIN_R_TRIG,
    PIN_R_ECHO,

//Left LEDS/VIBCOINS
    PIN_L_LED,
    PIN_L_VIB,

//Right LEDS/VIBCOINS
    PIN_R_LED,
    PIN_R_VIB,

    PIN_COUNT //10
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

typedef enum {
    RANGE_SUPA_CLOSE,
    RANGE_CLOSE,
    RANGE_MEDIUM,
} distance_range_t;

void bike_init(void);
uint8_t button_pressed(void);
uint16_t ultrasonic_read(uint8_t trig_id, uint8_t echo_id);
void bike_update(void);
distance_range_t get_range(uint16_t dist_cm);
