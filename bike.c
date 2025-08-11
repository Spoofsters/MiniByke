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

}

//SETTING INPUTS FOR THE ULTRASONICS 

