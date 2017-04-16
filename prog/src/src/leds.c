#include <avr/io.h>
#include <avr/interrupt.h>
#include "leds.h"

#define LED_BLUE_DDR       DDRD
#define LED_BLUE_PORT      PORTD
#define LED_BLUE_PNUM      1
#define LED_RED_DDR        DDRD
#define LED_RED_PORT       PORTD
#define LED_RED_PNUM       3

void initLeds()
{
    LED_BLUE_DDR |= _BV(LED_BLUE_PNUM);
    LED_RED_DDR |= _BV(LED_RED_PNUM);
}

void turnStatusLed(bool state)
{
    if(state)
        LED_BLUE_PORT |= _BV(LED_BLUE_PNUM);
    else
        LED_BLUE_PORT &=~ _BV(LED_BLUE_PNUM);
}

void turnPowerLed(bool state)
{
    if(state)
        LED_RED_PORT |= _BV(LED_RED_PNUM);
    else
        LED_RED_PORT &=~ _BV(LED_RED_PNUM);
}
