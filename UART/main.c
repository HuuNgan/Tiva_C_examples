#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "uartstdio.h"

#define red_led     GPIO_PIN_1
#define blue_led    GPIO_PIN_2
#define green_led   GPIO_PIN_3

//void UARTprintf(const char *pcString, ...);
//void UARTStdioConfig(uint32_t ui32Port, uint32_t ui32Baud,
//                            uint32_t ui32SrcClock);

bool blink_active = false;
uint8_t led;

void Systick_init(void)
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
    if(blink_active == true)
    {
        led ^= red_led;
        GPIOPinWrite(GPIO_PORTF_BASE, red_led, led);
    }
}

int main(void) {
    char receive_char;

    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    Systick_init();

    //Set red_led pin as GPIO - output
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, red_led);

    //Enable the UART peripheral
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    //Set the Rx/Tx pins as UART pins
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    //Configure the UART baud rate, data configuration
//    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

    UARTStdioConfig(0, 115200, SysCtlClockGet());
    UARTprintf("Hello! Please press: \n  O: Turn on RED LED\n  F: Turn off RED LED\n  T: Blink RED LED at 1Hz\n");

    while (1)
    {
        if (UARTCharsAvail(UART0_BASE))
        {
            receive_char = UARTCharGet(UART0_BASE);
            UARTCharPut(UART0_BASE, receive_char);

            if(receive_char == 'o' || receive_char == 'O')
            {
                blink_active = false;
                GPIOPinWrite(GPIO_PORTF_BASE, red_led, red_led);
            }
            else if(receive_char == 'f' || receive_char == 'F')
            {
                blink_active = false;
                GPIOPinWrite(GPIO_PORTF_BASE, red_led, !red_led);
            }
            else if(receive_char == 't' || receive_char == 'T')
                blink_active = true;
        }
    }
}
