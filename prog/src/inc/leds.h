#ifndef LEDS_H
#define LEDS_H

#include <stdbool.h>

void initLeds();
void turnStatusLed(bool state);
void turnPowerLed(bool state);

#endif //LEDS_H
