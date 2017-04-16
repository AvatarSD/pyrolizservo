#include <avr/io.h>
#include <avr/interrupt.h>
#include "adc.h"


// Voltage Reference: AVCC pin
#define ADC_VREF_TYPE ((0<<REFS1) | (1<<REFS0) | (0<<ADLAR))
#define ADC_INPUT 6

volatile uint16_t adc_data;

// ADC interrupt service routine
// with auto input scanning
ISR(ADC_vect)
{
    // Read the AD conversion result
    adc_data=ADCW;
    // Start the AD conversion
    ADCSRA|=(1<<ADSC);
}

uint16_t getPos()
{
    return adc_data;
}

void initAdc()
{
    // Analog Comparator initialization
    // Analog Comparator: Off
    // The Analog Comparator's positive input is
    // connected to the AIN0 pin
    // The Analog Comparator's negative input is
    // connected to the AIN1 pin
    ACSR=(1<<ACD) | (0<<ACBG) | (0<<ACO) | (0<<ACI) | (0<<ACIE) | (0<<ACIC) | (0<<ACIS1) | (0<<ACIS0);
    // Digital input buffer on AIN0: On
    // Digital input buffer on AIN1: On
    DIDR1=(0<<AIN0D) | (0<<AIN1D);

    // ADC initialization
    // ADC Clock frequency: 125,000 kHz
    // ADC Voltage Reference: AVCC pin
    // ADC Auto Trigger Source: Free Running
    // Digital input buffers on ADC0: On, ADC1: On, ADC2: On, ADC3: On
    // ADC4: On, ADC5: On
    DIDR0=(0<<ADC5D) | (0<<ADC4D) | (0<<ADC3D) | (0<<ADC2D) | (0<<ADC1D) | (0<<ADC0D);
    ADMUX=ADC_INPUT | ADC_VREF_TYPE;
    ADCSRA=(1<<ADEN) | (1<<ADSC) | (1<<ADATE) | (0<<ADIF) | (1<<ADIE) | (1<<ADPS2) | (1<<ADPS1) | (0<<ADPS0);
    ADCSRB=(0<<ADTS2) | (0<<ADTS1) | (0<<ADTS0);
}
