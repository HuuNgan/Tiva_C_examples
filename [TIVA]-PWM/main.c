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

int main( void )
{
    /*
     * System clock Divider: 5
     * Use PLL or OSC.
     * Select External Frequency: here it is 16 MHz.
     * Configure the system clock to run using a 16MHz crystal on the main oscillator,
     * driving the 400MHz PLL. The 400MHz PLL oscillates at only that frequency,
     * but can be driven by crystals or oscillators running between 5 and 25MHz.
     * There is a default /2 divider in the clock path and we are specifying another /5, which totals 10.
     * That means the System Clock will be 40MHz.
     *
     */
    SysCtlClockSet( SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ );

    //Divide system clock by 16 --> PWM clock = 1MHz
    SysCtlPWMClockSet( SYSCTL_PWMDIV_16 );

    //Enable the PWM peripheral, where we select PWM1 module.
    SysCtlPeripheralEnable( SYSCTL_PERIPH_PWM1 );

    //Enable GPIO pins. PD0 is M1PWM0
    SysCtlPeripheralEnable( SYSCTL_PERIPH_GPIOD );

    //Select the Pin Type for PWM
    GPIOPinTypePWM( GPIO_PORTD_BASE, GPIO_PIN_0 | GPIO_PIN_1 );

    //M1PWM0 corresponds to Motion Control Module 1 PWM 0.
    //This signal is controlled by Module 1 PWM Generator 0.
    //Similar to M1PWM1
    GPIOPinConfigure( GPIO_PD0_M1PWM0 );
    GPIOPinConfigure( GPIO_PD1_M1PWM1 );

    //This function is used to set the mode of operation for a PWM generator which can be counting mode,
    //synchronization mode, also a PWM generator can count in two different modes:
    //countdown mode or count up/down mode.
    PWMGenConfigure( PWM1_BASE, PWM_GEN_0, PWM_GEN_MODE_DOWN );
    PWMGenPeriodSet( PWM1_BASE, PWM_GEN_0, 1000 );     //Total Pulse Period is 1ms

    PWMPulseWidthSet( PWM1_BASE, PWM_OUT_0,  ( PWMGenPeriodGet(PWM1_BASE, PWM_GEN_0) / 2) );
    PWMPulseWidthSet( PWM1_BASE, PWM_OUT_1,  ( PWMGenPeriodGet(PWM1_BASE, PWM_GEN_0) / 20) );

    //Turn on the OUTPUT Pins
    PWMOutputState( PWM1_BASE, PWM_OUT_0_BIT | PWM_OUT_1_BIT, true );


    //Enable the PWM generator
    PWMGenEnable( PWM1_BASE, PWM_GEN_0 );

    while( 1 ) {
        SysCtlDelay(16000000);
        PWMPulseWidthSet( PWM1_BASE, PWM_OUT_0,  100 );
        PWMPulseWidthSet( PWM1_BASE, PWM_OUT_1,  100 );
    }
}
