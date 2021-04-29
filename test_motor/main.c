//*****************************************************************************
//----------------------------DC MOTOR CONTROL---------------------------------
//--------------------------Pay It Forward Club--------------------------------
//-------------------------Author: LUONG HUU NGAN------------------------------
//-------------------------Created on: May 7, 2019-----------------------------
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
#include "driverlib/systick.h"
#include "driverlib/interrupt.h"
#include "inc/hw_gpio.h"
#include "Motor/motor.h"

int speed = 1000;
int32_t sys_value;

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

void SysTickConfig(void)
{
    SysTickPeriodSet(SysCtlClockGet());

    // Enable interrupts to the processor.
    IntMasterEnable();
    // Enable the SysTick Interrupt.
    SysTickIntEnable();
    // Enable SysTick.
    SysTickEnable();
}

void SysTickIntHandler(void)
{
    sys_value = SysTickValueGet();
}

int main(void)
{
    //Initialize clock
    SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

    PWM_Config();
    MOTOR_Config();
    SysTickConfig();

    //Loop forever
    while(1)
    {
        setSpeed_A(speed);
        motorA_run(forward);
        SysCtlDelay(16666666);
        motorA_run(backward);
        SysCtlDelay(16666666);
        release_A();  release_B();

        setSpeed_B(speed/2);
        motorB_run(forward);
        SysCtlDelay(16666666);
        motorB_run(backward);
        SysCtlDelay(16666666);
        release_A();  release_B();
    }
}
