#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "inc/hw_gpio.h"

#define SW_PORT     GPIO_PORTF_BASE
#define SW1_PIN     GPIO_PIN_0
#define SW2_PIN     GPIO_PIN_4
#define LED_PORT    GPIO_PORTF_BASE
#define LED_RED     GPIO_PIN_1
#define LED_GREEN   GPIO_PIN_2
#define LED_BLUE    GPIO_PIN_3

void initTIVA()
{
    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
}

void initSwitchLed()
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
//    GPIOPinTypeGPIOOutput(LED_PORT, LED_RED | LED_GREEN | LED_BLUE);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
    GPIOPinTypeGPIOInput(SW_PORT, SW1_PIN | SW2_PIN);

    //Unlock GPIO PF0
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= GPIO_PIN_0;
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;

    //Pull-up SW
    GPIOPadConfigSet(SW_PORT, SW1_PIN | SW2_PIN, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
}

void initSysTick()
{
    SysTickPeriodSet(SysCtlClockGet());
    // Enable interrupts to the processor.
    IntMasterEnable();
    // Enable the SysTick Interrupt.
    SysTickIntEnable();
    // Enable SysTick.
    SysTickEnable();
}

void initUART0()
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 9600,
        (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

//    IntMasterEnable(); //enable processor interrupts
//    IntEnable(INT_UART0); //enable the UART interrupt
//    UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT); //only enable RX and TX interrupts
}

void putstr(char *str)
{
    uint8_t i=0;
    while(str[i] != 0)
    {
        UARTCharPut(UART0_BASE, str[i]);
        i++;
    }
}
void getstr(char *str)
{
    uint8_t i=0;
    char temp=0;
    while(temp != 0x0D)
    {
        if (UARTCharsAvail(UART0_BASE))
        {
            temp = UARTCharGet(UART0_BASE);
            str[i] = temp;
            i++;
//            UARTCharPut(UART0_BASE, temp);
        }
    }
    str[i] = 0;
}

void SysTickIntHandler(void)
{
    ;
}

int main(void)
{
    initTIVA();
    initSwitchLed();
    initSysTick();
    initUART0();

    char uart_rec[50];
    char temp1, temp2;

    while(1)
    {
        putstr("Nhap toan hang dau tien:\n");
        getstr(uart_rec);
        putstr(uart_rec);
    }
//	return 0;
}
