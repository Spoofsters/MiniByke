#include "bike.h"
#include "bike.c"

int main(void) {
    L_CONST = (10 / x);
    H_CONST = (44 / y);

    bike_init();
    timer0_init();

    while (1) {
        uint16_t dist1 = ultrasonic_read(PIN_1_TRIG, PIN_1_ECHO);
        uint16_t dist2 = ultrasonic_read(PIN_2_TRIG, PIN_2_ECHO);

        if (dist1 <= H_CONST || dist2 <= H_CONST) {
            PORTD |= (1 << VIBRATION_PIN);
        } else if (dist1 <= L_CONST || dist2 <= L_CONST) {
            PORTD |= (1 << LED_PIN);
        } else {
            PORTD &= ~((1 << LED_PIN) | (1 << VIBRATION_PIN));
        }
    }
}