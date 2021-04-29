#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "switches.h"
#include "led.h"
#include <ledGreenStateMachine.h>
#include <ledRedStateMachine.h>

uint16_t time = 10;

bool event_Red, event_Green;

void SysTickIntHandler(void)
{
    if (ledGTimer>0)    ledGTimer--;
    time --;
    if(time==0)
    {
        time =10;
        event_Red = 1;
        event_Green = 0;
    }
    if(time==5)
    {
        event_Red = 0;
        event_Green = 1;
    }
}

int main(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    SysTickPeriodSet(SysCtlClockGet()/1000);
    IntMasterEnable();      // Enable interrupts to the processor.
    SysTickIntEnable();     // Enable the SysTick Interrupt.
    SysTickEnable();        // Enable SysTick.

    switchInit();
    ledInit();

    while(1)
    {
        if(event_Red) ledRedStateMachineUpdate();
        if(event_Green) ledGreenStateMachineUpdate();
    }
}
