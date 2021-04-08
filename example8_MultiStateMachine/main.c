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

void SysTickIntHandler(void)
{
    if (ledGTimer>0)    ledGTimer--;

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
        ledRedStateMachineUpdate();
        ledGreenStateMachineUpdate();
    }
}
