//*****************************************************************************
//
// freertos_demo.c - Simple FreeRTOS example.
//
// Copyright (c) 2012-2017 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
// 
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
// 
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
// 
// This is part of revision 2.1.4.178 of the EK-TM4C123GXL Firmware Package.
//
//*****************************************************************************

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "led_task.h"
#include "switch_task.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#define TARGET_IS_BLIZZARD_RB1
#include "driverlib/rom.h"
#include "driver/led.h"
#include "driver/switches.h"
#include "driverlib/interrupt.h"
#include "inc/hw_nvic.h"
//*****************************************************************************
//
//! \addtogroup example_list
//! <h1>FreeRTOS Example (freertos_demo)</h1>
//!
//! This application demonstrates the use of FreeRTOS on Launchpad.
//!
//! The application blinks the user-selected LED at a user-selected frequency.
//! To select the LED press the left button and to select the frequency press
//! the right button.  The UART outputs the application status at 115,200 baud,
//! 8-n-1 mode.
//!
//! This application utilizes FreeRTOS to perform the tasks in a concurrent
//! fashion.  The following tasks are created:
//!
//! - An LED task, which blinks the user-selected on-board LED at a
//!   user-selected rate (changed via the buttons).
//!
//! - A Switch task, which monitors the buttons pressed and passes the
//!   information to LED task.
//!
//! In addition to the tasks, this application also uses the following FreeRTOS
//! resources:
//!
//! - A Queue to enable information transfer between tasks.
//!
//! - A Semaphore to guard the resource, UART, from access by multiple tasks at
//!   the same time.
//!
//! - A non-blocking FreeRTOS Delay to put the tasks in blocked state when they
//!   have nothing to do.
//!
//! For additional details on FreeRTOS, refer to the FreeRTOS web page at:
//! http://www.freertos.org/
//
//*****************************************************************************


#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
}

#endif

//*****************************************************************************
//
// This hook is called by FreeRTOS when an stack overflow error is detected.
//
//*****************************************************************************
void
vApplicationStackOverflowHook(xTaskHandle *pxTask, char *pcTaskName)
{
    //
    // This function can not return, so loop forever.  Interrupts are disabled
    // on entry to this function, so no processor interrupts will interrupt
    // this loop.
    //
    while(1)
    {
    }
}

//*****************************************************************************
//
// Configure the UART and its pins.  This must be called before UARTprintf().
//
//*****************************************************************************
void
ConfigureUART(void)
{
    //
    // Enable the GPIO Peripheral used by the UART.
    //
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    //
    // Enable UART0
    //
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    //
    // Configure GPIO Pins for UART mode.
    //
    ROM_GPIOPinConfigure(GPIO_PA0_U0RX);
    ROM_GPIOPinConfigure(GPIO_PA1_U0TX);
    ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    //
    // Use the internal 16MHz oscillator as the UART clock source.
    //
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);

    //
    // Initialize the UART for console I/O.
    //
    UARTStdioConfig(0, 115200, 16000000);
}


xSemaphoreHandle xBinarySemaphore;

static void vHandlerTask( void *pvParameters );
static void vPeriodicTask( void *pvParameters );

//*****************************************************************************
//
// Initialize FreeRTOS and start the initial set of tasks.
//
//*****************************************************************************
int main(void)
{

        SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ |
                       SYSCTL_OSC_MAIN);
      SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    ConfigureUART();
        IntMasterDisable();
        switchInit();
        swIntEnable();

        vSemaphoreCreateBinary( xBinarySemaphore );

    xTaskCreate( vHandlerTask, "Handler", 240, NULL, 2, NULL );
    xTaskCreate( vPeriodicTask, "Periodic", 240, NULL, 1, NULL );
    vTaskStartScheduler();
    for( ;; );
}

void GPIOPortFIntHandler()
{
        portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
        UARTprintf( "GPIOPortFIntHandler - About to give Semaphore\n" );
    xSemaphoreGiveFromISR( xBinarySemaphore, &xHigherPriorityTaskWoken );
        GPIOIntClear(BUTTONS_GPIO_BASE, ALL_BUTTONS);
    portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
}
static void vPeriodicTask( void *pvParameters )
{
       for( ;; )
    {
        /* This task is just used to 'simulate' an interrupt.  This is done by
        periodically generating a software interrupt. */
        vTaskDelay( 5000 / portTICK_RATE_MS );
        /* Generate the interrupt, printing a message both before hand and
        afterwards so the sequence of execution is evident from the output. */
         UARTprintf( "Periodic task - About to generate interrupt #1.\n" );
         HWREG(NVIC_SW_TRIG) = INT_GPIOF - 16;
               UARTprintf( "Periodic task - About to generate interrupt #2.\n" );
         HWREG(NVIC_SW_TRIG) = INT_GPIOF - 16;
         UARTprintf( "Periodic task - Interrupt generated.\n" );
    }
}
static void vHandlerTask( void *pvParameters )
{
    xSemaphoreTake( xBinarySemaphore, 0 );
    for( ;; )
    {
                UARTprintf( "Handler task - Trying to take semaphore\n" );
        xSemaphoreTake( xBinarySemaphore, portMAX_DELAY );
        UARTprintf( "Handler task - Processing event.\n\n" );
    }
}
