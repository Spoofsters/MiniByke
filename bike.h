#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>

#define PIN_INPUT   0
#define PIN_OUTPUT  1
#define PULL_DISABLED 0
#define PULL_ENABLED  1
//DEFINING NUMBERS NEEDED FOR LOW AND HIGH CONSTANTS
#define x 211.2 // WE CAN REPLACE WITH ACCELEROMETER. CURRENTLY IN INCHES PER SECOND
#define y 17.6

uint32_t ONGOING_TIMER;
uint32_t CURRENT_TIMER;
uint32_t START_TIMER; 

<<<<<<< HEAD
//WHIMSICAL FUNCTIONS!
void timer0_init(); //Use Timer0
void millis(); //timer function in milliseconds
=======
typedef struct {
    volatile uint8_t *ddr;
    volatile uint8_t *port;
    volatile uint8_t *pin_reg;
    uint8_t bit;
    uint8_t dir; //Indicator if pin is input or output
    uint8_t pull; //Indicates if pull up resistor enabled or not
} pin_def_t;

typedef enum {
    PIN_BUTTON,
    PIN_L_TRIG,
    PIN_L_ECHO,
    PIN_R_TRIG,
    PIN_R_ECHO,
    PIN_L_LED,
    PIN_L_VIB,
    PIN_R_LED,
    PIN_R_VIB,
    PIN_COUNT
} pin_id_t;

//WHIMSICAL FUNCTIONS!
void timer0_init(void); //Use Timer0
uint32_t millis(void); //timer function in milliseconds
>>>>>>> 3be5dae (fixed redundant errors appearing in lookup table)

void bike_init(void);
uint8_t button_pressed(void);
uint16_t ultrasonic_read(uint8_t trig_id, uint8_t echo_id);
void bike_update(void);
