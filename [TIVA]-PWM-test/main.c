//*****************************************************************************
//----------------------------DC MOTOR CONTROL---------------------------------
//--------------------------Pay It Forward Club--------------------------------
//-------------------------Author: LUONG HUU NGAN------------------------------
//------------------------------------------Date: 5/5/2019---------------------
//-------------------------Feel Free to copy it--------------------------------
//*****************************************************************************
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/debug.h"
#include "driverlib/pwm.h"
#include "driverlib/pin_map.h"
#include "inc/hw_gpio.h"

//*****************************************************************************
//This program using these Pins:
//PC4, PC5, PC6, PC7, PD0, PD1
//If you want to change PORT, remember to check PWM_GEN.

#define MOTOR_PORT       GPIO_PORTC_BASE
#define motorA_Pin1      GPIO_PIN_4
#define motorA_Pin2      GPIO_PIN_5
#define motorB_Pin1      GPIO_PIN_6
#define motorB_Pin2      GPIO_PIN_7
#define MOTOR_PERIPH     SYSCTL_PERIPH_GPIOC

#define PWM_PORT         GPIO_PORTD_BASE
#define PWM_Pin_A        GPIO_PIN_0
#define PWM_Pin_B        GPIO_PIN_1
#define PWM_PERIPH       SYSCTL_PERIPH_GPIOD
#define PWM_A_Config     GPIO_PD0_M1PWM0
#define PWM_B_Config     GPIO_PD1_M1PWM1
#define PWM_OUT_A        PWM_OUT_0
#define PWM_OUT_B        PWM_OUT_1
#define PWM_OUT_A_BIT    PWM_OUT_0_BIT
#define PWM_OUT_B_BIT    PWM_OUT_1_BIT

#define backward  'b'
#define forward  'f'

//*****************************************************************************
void motorA_run(char direction);
void motorB_run(char direction);
void release_all(void);
void setSpeed_A(int speed);
void setSpeed_B(int speed);

//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
    while(1);
}
#endif

//*****************************************************************************

int main(void)
{
    //Initialize clock
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

    //Divide system clock by 16 --> PWM clock = 1MHz
    SysCtlPWMClockSet( SYSCTL_PWMDIV_16 );

    //Enable the PWM peripheral, where we select PWM1 module.
    SysCtlPeripheralEnable( SYSCTL_PERIPH_PWM1 );

    // Enable the peripheral access. PD0 is M1PWM0
    SysCtlPeripheralEnable(PWM_PERIPH);
    SysCtlPeripheralEnable(MOTOR_PERIPH);

    // Enable the GPIO pin for the motors.  Set the direction as output, and
    // enable the GPIO pin for digital function.
    GPIOPinTypeGPIOOutput(MOTOR_PORT,motorA_Pin1|motorA_Pin2|motorB_Pin1|motorB_Pin2);
    GPIOPinTypePWM(PWM_PORT,PWM_Pin_A | PWM_Pin_B);    //Select the Pin Type for PWM

    //M1PWM0 corresponds to Motion Control Module 1 PWM 0.
    //This signal is controlled by Module 1 PWM Generator 0.
    //Similar to M1PWM1
    GPIOPinConfigure( PWM_A_Config );
    GPIOPinConfigure( PWM_B_Config );

    //This function is used to set the mode of operation for a PWM generator which can be counting mode,
    //synchronization mode, also a PWM generator can count in two different modes:
    //countdown mode or count up/down mode.
    PWMGenConfigure( PWM1_BASE, PWM_GEN_0, PWM_GEN_MODE_DOWN );
    PWMGenPeriodSet( PWM1_BASE, PWM_GEN_0, 100 );     //Total Pulse Period is 0.1ms

    //Set speed to 1. Speed range from 0 to 100.
    //If you set speed to 0, it will work the same as 100 (max value).
    PWMPulseWidthSet( PWM1_BASE, PWM_OUT_A,  1 );
    PWMPulseWidthSet( PWM1_BASE, PWM_OUT_B,  1 );

    //Turn on the OUTPUT Pins
    PWMOutputState( PWM1_BASE, PWM_OUT_A_BIT | PWM_OUT_B_BIT, true );


    //Enable the PWM generator
    PWMGenEnable( PWM1_BASE, PWM_GEN_0 );

    //Loop forever
    while(1)
    {
        setSpeed_A(100);
        motorA_run(forward);
        SysCtlDelay(16000000);
        motorA_run(backward);
        SysCtlDelay(16000000);
        release_all();

        setSpeed_B(50);
        motorB_run(forward);
        SysCtlDelay(16000000);
        motorB_run(backward);
        SysCtlDelay(16000000);
        release_all();
    }
}

//*****************************************************************************
void setSpeed_A(int speed)
{
    //speed range from 1 to 100
    PWMPulseWidthSet( PWM1_BASE, PWM_OUT_A,  speed );

}

void setSpeed_B(int speed)
{
    //speed range from 1 to 100
    PWMPulseWidthSet( PWM1_BASE, PWM_OUT_B,  speed );
}

void motorA_run(char direction)
{
    if (direction==forward)
    {
        GPIOPinWrite(MOTOR_PORT, motorA_Pin1, motorA_Pin1);
        GPIOPinWrite(MOTOR_PORT, motorA_Pin2, 0x00);
    }
    else if (direction==backward)
    {
        GPIOPinWrite(MOTOR_PORT, motorA_Pin1, 0x00);
        GPIOPinWrite(MOTOR_PORT, motorA_Pin2, motorA_Pin2);
    }
}

void motorB_run(char direction)
{
    if (direction==forward)
    {
        GPIOPinWrite(MOTOR_PORT, motorB_Pin1, motorB_Pin1);
        GPIOPinWrite(MOTOR_PORT, motorB_Pin2, 0x00);
    }
    else if (direction==backward)
    {
        GPIOPinWrite(MOTOR_PORT, motorB_Pin1, 0x00);
        GPIOPinWrite(MOTOR_PORT, motorB_Pin2, motorB_Pin2);
    }
}

void release_all(void)
{
    GPIOPinWrite(MOTOR_PORT, motorA_Pin1|motorA_Pin2|motorB_Pin1|motorB_Pin2, 0x00);
}
