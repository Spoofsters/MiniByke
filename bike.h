#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stddef.h>
#include <stdarg.h>
#include <assert.h>
#include <stdint.h>
#include <util/twi.h>
#include <cys.clk.h>

uint8_t ONGOING_TIMER;
uint8_t CURRENT_TIMER;
uint8_t START_TIMER;

void TIMER_0(); //Use Timer0
void millis(); //timer function in milliseconds

enum {
    L_CONST,
    H_CONST,
    TOTAL,
 } BOUNDARIES