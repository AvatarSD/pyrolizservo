#include <avr/io.h>
#include <avr/interrupt.h>
#include "motor.h"
#include <leds.h>

#include <util/delay.h>

#define MAX_POWER 0x3F // 0x00-0xFF

#define EN1_DDR            DDRD
#define EN1_PORT           PORTD
#define EN1_PNUM           7

#define EN2_DDR            DDRD
#define EN2_PORT           PORTD
#define EN2_PNUM           4

#define PWM1               OCR0A
#define PWM1_DDR           DDRD
#define PWM1_PNUM          6

#define PWM2               OCR0B
#define PWM2_DDR           DDRD
#define PWM2_PNUM          5


void initMotor()
{
    // Port initialization
    EN1_DDR |= _BV(EN1_PNUM);
    EN2_DDR |= _BV(EN2_PNUM);
    PWM1_DDR |= _BV(PWM1_PNUM);
    PWM2_DDR |= _BV(PWM2_PNUM);

    // Timer/Counter 0 initialization
    // Clock source: System Clock
    // Clock value: 125,000 kHz
    // Mode: Phase correct PWM top=0xFF
    // OC0A output: Non-Inverted PWM
    // OC0B output: Non-Inverted PWM
    // Timer Period: 4,08 ms
    // Output Pulse(s):
    // OC0A Period: 4,08 ms Width: 0 us// OC0B Period: 4,08 ms Width: 0 us
    TCCR0A=(1<<COM0A1) | (0<<COM0A0) | (1<<COM0B1) | (0<<COM0B0) | (0<<WGM01) | (1<<WGM00);
    TCCR0B=(0<<WGM02) | (0<<CS02) | (1<<CS01) | (1<<CS00);
    TCNT0=0x00;
    OCR0A=0x00;
    OCR0B=0x00;

    // Timer/Counter 0 Interrupt(s) initialization
    TIMSK0=(0<<OCIE0B) | (0<<OCIE0A) | (1<<TOIE0);
}

// Timer 0 overflow interrupt service routine
ISR(TIMER0_OVF_vect)
{
    // Place your code here
    if(PWM1 > 0)
    {
        EN1_PORT &=~ (1<<EN1_PNUM);
        EN2_PORT |= (1<<EN2_PNUM);
    }
    else if(PWM2 > 0)
    {
        EN2_PORT &=~ (1<<EN2_PNUM);
        EN1_PORT |= (1<<EN1_PNUM);;
    }
    else
    {
        EN2_PORT &=~ (1<<EN2_PNUM);
        EN1_PORT &=~ (1<<EN1_PNUM);
    }
}

void setOutput(int16_t val)
{
    if(val > MAX_POWER) val = MAX_POWER;
    if(val < -MAX_POWER) val = -MAX_POWER;

    if(val > 0)
    {
        turnPowerLed(true);
        EN1_PORT &=~ (1<<EN1_PNUM);
        PWM2 = 0x00;
        PWM1 = val;
    }
    else if(val < 0)
    {
        turnPowerLed(true);
        EN2_PORT &=~ (1<<EN2_PNUM);
        PWM1 = 0x00;
        PWM2 = -val;
    }
    else
    {
        turnPowerLed(false);
        EN2_PORT &=~ (1<<EN2_PNUM);
        EN1_PORT &=~ (1<<EN1_PNUM);
        PWM2 = 0x00;
        PWM1 = 0x00;
    }
}
