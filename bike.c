#include "bike.h"
uint32_t milliseconds = 0;

//DEFINING NUMBERS NEEDED FOR LOW AND HIGH CONSTANTS
int x = 211.2 // WE CAN REPLACE WITH ACCELEROMETER. CURRENTLY IN INCHES PER SECOND
int y = 17.6

// TIMER FUNCTIONALITY


void TIMER_0() {
  TCCR0B = (1 << CS01) | (1 << CS00); //Prescaler 64
  TIMSK0 = (1 << TOIE0); //Initialize timer0
}

ISR(TIMER0_OVF_vect) {
    milliseconds++;
}

uint32_t Millis() {
    uint32_t ms;
    uint8_t oldSREG = SREG;
    ms = milliseconds;
    cli();
    oldSREG = SREG;
    return ms;

}

//INPUT AND OUTPUT COMMANDS FOR SENSORS

L_CONST = (10/x); //FILTER MAXIMUM
H_CONST = (44/y); //VIBRATION MINIMUM


