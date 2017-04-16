#include <avr/io.h>
#include <avr/interrupt.h>
#include "input.h"
#include <leds.h>


#define INPUT_DDR DDRD
#define INPUT_PIN PIND
#define INPUT_NUM 2
#define INPUT_VAL (INPUT_PIN & _BV(INPUT_NUM))

#define TIMER_VAL TCNT1
#define TIMERSTEP_IN_MS 8

#define LEGAL_VAL_MAX 2000
#define LEGAL_VAL_MIN 1000

volatile uint16_t value;
volatile bool isValid = false;
volatile bool active = false;

static void setActive(bool val)
{
    active = val;
    turnStatusLed(val);
}

static void setValue(uint16_t val)
{
    val /= TIMERSTEP_IN_MS;
    if((val >= LEGAL_VAL_MIN) && (val <= LEGAL_VAL_MAX))
    {
        value = val - LEGAL_VAL_MIN;
        setActive(true);
    }
    else
        setActive(false);
}

// External Interrupt 0 service routine
ISR(INT0_vect)
{
    bool input = INPUT_VAL;
    uint16_t timer = TIMER_VAL;

    if(input){
        TIMER_VAL = 0;
        isValid = true;
    }
    else{
        if(isValid)
            setValue(timer);
        else
            setActive(false);
        isValid = false;
    }

}

// Timer1 overflow interrupt service routine
ISR(TIMER1_OVF_vect)
{
    // Place your code here
    isValid = false;
    setActive(false);
}

void initInput()
{
    // Timer/Counter 1 initialization
    // Clock source: System Clock
    // Clock value: 8000,000 kHz
    // Mode: Normal top=0xFFFF
    // OC1A output: Disconnected
    // OC1B output: Disconnected
    // Noise Canceler: Off
    // Input Capture on Falling Edge
    // Timer Period: 8,192 ms
    // Timer1 Overflow Interrupt: On
    // Input Capture Interrupt: Off
    // Compare A Match Interrupt: Off
    // Compare B Match Interrupt: Off
    TCCR1A=(0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<WGM11) | (0<<WGM10);
    TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (0<<WGM12) | (0<<CS12) | (0<<CS11) | (1<<CS10);
    TCNT1H=0x00;
    TCNT1L=0x00;
    ICR1H=0x00;
    ICR1L=0x00;
    OCR1AH=0x00;
    OCR1AL=0x00;
    OCR1BH=0x00;
    OCR1BL=0x00;

    // Timer/Counter 1 Interrupt(s) initialization
    TIMSK1=(0<<ICIE1) | (0<<OCIE1B) | (0<<OCIE1A) | (1<<TOIE1);



    // External Interrupt(s) initialization
    // INT0: On
    // INT0 Mode: Any change
    // INT1: Off
    // Interrupt on any change on pins PCINT0-7: Off
    // Interrupt on any change on pins PCINT8-14: Off
    // Interrupt on any change on pins PCINT16-23: Off
    EICRA=(0<<ISC11) | (0<<ISC10) | (0<<ISC01) | (1<<ISC00);
    EIMSK=(0<<INT1) | (1<<INT0);
    EIFR=(0<<INTF1) | (1<<INTF0);
    PCICR=(0<<PCIE2) | (0<<PCIE1) | (0<<PCIE0);

}

uint16_t getInputVal()
{
    return value;
}

bool isActive()
{
    return active;
}
