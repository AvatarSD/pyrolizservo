/*******************************************************
This program was created by the
CodeWizardAVR V3.12 Advanced
Automatic Program Generator
╘ Copyright 1998-2014 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

Project : AvtomotiveServo
Version : 1.0
Date    : 13.04.2017
Author  : S.D.
Company : avatarsd.com
Comments: 


Chip type               : ATmega168PA
Program type            : Application
AVR Core Clock frequency: 8,000000 MHz
Memory model            : Small
External RAM size       : 0
Data Stack size         : 256
*******************************************************/

#include <adc.h>
#include <input.h>
#include <leds.h>
#include <motor.h>

#include <avr/interrupt.h>
#include <util/delay.h>


#define Kp 1.2
#define Ki 0.002
#define Kd 2.4
#define INTERVAL 5
#define DEADZONE 9

int16_t pid_control(int16_t setpoint, int16_t currentpoint)
{
    static int16_t last_error;
    static int16_t I;
    int16_t P, D;
    P = (setpoint - currentpoint);
    if((P < DEADZONE) && (P > -DEADZONE)) return 0;
    I = (I + (setpoint - currentpoint) * INTERVAL);
    D = (((setpoint - currentpoint) - last_error) / INTERVAL);
    last_error = setpoint - currentpoint;
    return (Kp * P) + (Ki * I) + (Kd * D);
}

int main(void)
{
    initLeds();
    initAdc();
    initMotor();
    initInput();

    // Global enable interrupts
    sei();

    while (1)
        if(isActive()){
            setOutput(pid_control(getInputVal(), (getPos())));
            _delay_ms(INTERVAL);
        }
        else
            setOutput(0);

    return 0;
}
