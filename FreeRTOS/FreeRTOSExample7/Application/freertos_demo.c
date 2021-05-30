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


#define     TASK1       1
#define     TASK2       2
/* The two task functions. */
static  void vButtonLeftTask( void *pvParameters );
static  void vButtonRightTask( void *pvParameters );
static  void vReceiverTask( void *pvParameters );

/* Declare a variable of type xQueueHandle.  This is used to store the queue
that is accessed by all three tasks. */
xQueueHandle xQueue;

/* Define the structure type that will be passed on the queue. */
typedef struct
{
    sw_t buttonValue;
    unsigned int taskSource;
} Data_t;




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
    switchInit();
    xQueue = xQueueCreate( 3, sizeof( Data_t ) );

    xTaskCreate( vButtonLeftTask, "Sender 1", 1000, NULL, 1, NULL );
    xTaskCreate( vButtonRightTask, "Sender 2", 1000, NULL, 1, NULL );
    xTaskCreate( vReceiverTask, "Receiver", 1000, NULL, 2, NULL );
    vTaskStartScheduler();
    for( ;; );
}

static  void vButtonLeftTask( void *pvParameters )
{
    portBASE_TYPE xStatus;
    const portTickType xTicksToWait = 100 / portTICK_RATE_MS;
    sw_t    preVal, currentVal;
    Data_t  data;

    data.taskSource = TASK1;
    preVal = switchState(1);
    for( ;; )
    {
        currentVal = switchState(1);
        if (currentVal != preVal)
        {
                data.buttonValue = currentVal;
                preVal = currentVal;
              xStatus = xQueueSend( xQueue, &data, xTicksToWait );
                if( xStatus != pdPASS )
                {
                    UARTprintf( "Task 1 could not send to the queue.\n" );
                }
        }
    }
}
static  void vButtonRightTask( void *pvParameters )
{
    portBASE_TYPE xStatus;
    const portTickType xTicksToWait = 100 / portTICK_RATE_MS;
    sw_t    preVal, currentVal;
    Data_t  data;

    data.taskSource = TASK2;
    preVal = switchState(2);
    for( ;; )
    {
        currentVal = switchState(2);
        if (currentVal != preVal)
        {
                data.buttonValue = currentVal;
                preVal = currentVal;
              xStatus = xQueueSend( xQueue, &data, xTicksToWait );
                if( xStatus != pdPASS )
                {
                    UARTprintf( "Task 2 could not send to the queue.\n" );
                }
        }
    }
}
static void vReceiverTask( void *pvParameters )
{
/* Declare the structure that will hold the values received from the queue. */
    Data_t   data;
    portBASE_TYPE xStatus;
    const portTickType xTicksToWait = 5000 / portTICK_RATE_MS;

    /* This task is also defined within an infinite loop. */
    for( ;; )
    {
//      xStatus = xQueueReceive( xQueue, &data, xTicksToWait );
        xStatus = xQueueReceive( xQueue, &data, portMAX_DELAY );
        if( xStatus == pdPASS )
        {
            /* Data was successfully received from the queue, print out the received
            value and the source of the value. */
            if( data.taskSource == TASK1 )
            {
                UARTprintf( "From Sender 1: Left Button = %1d\n", data.buttonValue );
            }
            else
            {
                UARTprintf( "From Sender 2: Right Button = %1d\n", data.buttonValue );
            }
        }
        else
        {
            UARTprintf( "Could not receive from the queue in 5  sec\n" );
        }
    }
}
