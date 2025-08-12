#include "bike.h"

uint32_t milliseconds = 0;


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

} //Timer function

//SETTING INPUTS FOR THE ULTRASONICS 
static const pin_def_t pin_map[PIN_COUNT] = {
    [PIN_BUTTON]    = {&DDRD, &PORTD, &PIND, PD2, PIN_INPUT,  PULL_ENABLED},
    [PIN_L_TRIG]    = {&DDRD, &PORTD, &PIND, PD3, PIN_OUTPUT, PULL_DISABLED},
    [PIN_L_ECHO]    = {&DDRD, &PORTD, &PIND, PD4, PIN_INPUT,  PULL_DISABLED},
    [PIN_R_TRIG]    = {&DDRD, &PORTD, &PIND, PD5, PIN_OUTPUT, PULL_DISABLED},
    [PIN_R_ECHO]    = {&DDRD, &PORTD, &PIND, PD6, PIN_INPUT,  PULL_DISABLED},
    [PIN_L_LED]      = {&DDRD, &PORTD, &PIND, PD7, PIN_OUTPUT, PULL_DISABLED},
    [PIN_L_VIB]      = {&DDRB, &PORTB, &PINB, PB0, PIN_OUTPUT, PULL_DISABLED},
    [PIN_R_LED]      = {&DDRB, &PORTB, &PINB, PB1, PIN_OUTPUT, PULL_DISABLED},
    [PIN_R_VIB]      = {&DDRB, &PORTB, &PINB, PB2, PIN_OUTPUT, PULL_DISABLED}
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

    while (*(pin_map[echo_id].pin_reg) & (1<< pin_map[echo_id].bit)) {
        _delay_us(1);
        count_us++; 
    }

    return (count_us * 0.0343) / 2;
}

void bike_update(void) {
    static uint32_t PREV_TIME = 0;
    uint32_t CURRENT_TIME = millis();

    uint16_t DIST_LEFT = ultrasonic_read(PIN_L_TRIG, PIN_L_ECHO);
    uint16_t DIST_RIGHT = ultrasonic_read(PIN_R_TRIG, PIN_R_ECHO);

    //Maybe we can use ultrasonicread here and then use functions with else or if statements
    //since if using switch cases disables us from using changing variables, like millis.
}

void WhoGoesThere(void) 
{
    while (millis()<= 1000){

    switch(count_us){
        case count_us <= y: {
            break; //Filters out anything in sensor for shorter than Y
        };

        case count_us <= x: {
            PORTD |= 1<<VIBRATION_PIN; //Turns Vibration on
            break;
        };

       default:{
            PORTD |= 1<<LED_PIN; //Turns LED on 
            break;
        };
    }
}
}


//     // Reset the timer or perform any other necessary actions
//     PREV_TIME = CURRENT_TIME;
//     Optionally, you can turn off the LED or vibration her
//     PORTD &= ~(1 << VIBRATION_PIN); // Turns Vibration off
//     PORTD &= ~(1 << LED_PIN); // Turns LED off

// SAMPLE CODE TO TURN ON/OFF LED OR VIBRATION COINS:
// LED on:
// *pin_map[PIN_L_LED].port |= (1 << pin_map[PIN_L_LED].bit);

// vibrationCoin off!!
// *pin_map[PIN_R_VIB].port &= ~(1 << pin_map[PIN_R_VIB].bit);