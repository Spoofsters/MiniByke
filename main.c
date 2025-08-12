#include "bike.h"
#include "bike.c"

int main(void) {
    float L_CONST = (10 / x);
    float H_CONST = (44 / y);

    bike_init();
    timer0_init();

    while (1) {
       bike_update();
    }
}