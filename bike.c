#include "bike.h"
uint32_t milliseconds = 0;

//DEFINING NUMBERS NEEDED FOR LOW AND HIGH CONSTANTS
int x = 211.2 // WE CAN REPLACE WITH ACCELEROMETER. CURRENTLY IN INCHES PER SECOND
int y = 17.6
  
L_CONST = (10/x); //FILTER MAXIMUM
H_CONST = (44/y); //VIBRATION MINIMUM

// TIMER FUNCTIONALITY


void timer0_init(void) {
  TCCR0A = 0;
  TCCR0B = (1 << CS01) | (1 << CS00); //Prescaler 64
  TIMSK0 = (1 << TOIE0); //Initialize timer0
  sei();
}

ISR(TIMER0_OVF_vect) {
    milliseconds++;
}

uint32_t millis(void) {
    uint32_t ms;
    uint8_t oldSREG = SREG;
    cli();
    ms = milliseconds;
    SREG = oldSREG;
    return ms;

}

//SETTING INPUTS FOR THE ULTRASONICS 
static const pin_def_t pin_map[PIN_COUNT] = {
    [PIN_BUTTON]    = { &DDRD, &PORTD, &PIND, PD2, PIN_INPUT,  PULL_ENABLED },
    [PIN_1_TRIG]    = { &DDRD, &PORTD, &PIND, PD3, PIN_OUTPUT, PULL_DISABLED },
    [PIN_1_ECHO]    = { &DDRD, &PORTD, &PIND, PD4, PIN_INPUT,  PULL_DISABLED },
    [PIN_2_TRIG]    = { &DDRD, &PORTD, &PIND, PD5, PIN_OUTPUT, PULL_DISABLED },
    [PIN_2_ECHO]    = { &DDRD, &PORTD, &PIND, PD6, PIN_INPUT,  PULL_DISABLED }
};

void bike_init(void) {
    for (uint8_t i = 0; i < PIN_COUNT; i++) {
        if (pin_map[i].dir == PIN_OUTPUT) {
            *pin_map[i].ddr |= (1 << pin_map[i].bit);  //OUTPUTS pin address
        } else {
            *pin_map[i].ddr &= ~(1 << pin_map[i].bit); //INPUTS by pin address
            if (pin_map[i].pull == PULL_ENABLED) {
                *pin_map[i].port |= (1 << pin_map[i].bit); // enable pull-up
            } else {
                *pin_map[i].port &= ~(1 << pin_map[i].bit); // disable pull-up
            }
        }
    }
}

uint16_t ultrasonic_read(uint8_t trig_id, uint8_t echo_id) {
    uint32_t count_us = 0;
    // Trigger pulse
    *(pin_map[trig_id].port) &= ~(1 << pin_map[trig_id].bit);
    _delay_us(2);
    *(pin_map[trig_id].port) |= (1 << pin_map[trig_id].bit);
    _delay_us(10);
    *(pin_map[trig_id].port) &= ~(1 << pin_map[trig_id].bit);


    //Echo needs to go High before counting
    while (!(*(pin_map[echo_id].pin_reg) & (1 << pin_map[echo_id].bit)));
}

distance_range_t get_range(uint16_t dist_cm) {
    if (dist_cm < 10)  return RANGE_VERY_CLOSE;
    if (dist_cm < 30)  return RANGE_CLOSE;
    if (dist_cm < 100) return RANGE_MEDIUM;
    return RANGE_FAR;
}