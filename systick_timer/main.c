#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
//#include "driverlib/uart.h"
//#include "utils/uartstdio.h"
//#include "state.h"

volatile uint32_t g_ui32Counter = 0;
uint8_t ui8LED;
uint8_t MyTimerCount = 0xFF;

void SysTickIntHandler(void)
{
    //toggle led
    static uint8_t i;
    if(i<3)    i++;
    else if(i==3) i=0;

    ui8LED ^= GPIO_PIN_1;
    if(i==2) ui8LED ^= GPIO_PIN_3;
}

int main(void)
{

    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |SYSCTL_XTAL_16MHZ);

    SysTickPeriodSet(SysCtlClockGet());

    // Enable interrupts to the processor.
    IntMasterEnable();

    // Enable the SysTick Interrupt.
    SysTickIntEnable();

    // Enable SysTick.
    SysTickEnable();

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);

    while(1)
    {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, ui8LED);
    }
}
