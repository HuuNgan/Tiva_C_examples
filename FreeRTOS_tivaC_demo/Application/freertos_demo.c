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
//#include "led_task.h"
//#include "switch_task.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
//#define TARGET_IS_BLIZZARD_RB1
#include "driverlib/rom.h"
#include "drivers/led.h"
#include "drivers/switches.h"


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
// Initialize FreeRTOS and start the initial set of tasks.
//
//*****************************************************************************

//static void LEDGreenTask(void *pvParameters);
//static void LEDBlueTask(void *pvParameters);
static void LEDTask(void *pvParameters);
static void SWTask(void *pvParameters);
static void UARTTask(void *pvParameters);


//*****************************************************************************
//
// Initialize FreeRTOS and start the initial set of tasks.
//
//*****************************************************************************
int main(void)
{

        SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ |
                       SYSCTL_OSC_MAIN);
        ledInit();
        switchInit();
        xTaskCreate( LEDTask, "Task 1", configMINIMAL_STACK_SIZE, NULL,1, NULL );
        xTaskCreate( SW1Task,   "Task 2", configMINIMAL_STACK_SIZE, NULL, 1, NULL );
        xTaskCreate( SW2Task,   "Task 2", configMINIMAL_STACK_SIZE, NULL, 1, NULL );

    vTaskStartScheduler();
    while(1)
    {
    }
}

static void LEDTask(void *pvParameters)
{
    while(1)
    {
        ledControl(LEDRED, ON);
        vTaskDelay(500);
        ledControl(LEDRED, OFF);
        vTaskDelay(500);
    }
}

static void SWTask(void *pvParameters)
{
    static uint8_t sw_press_num=0;
    sw_t PreSW2State = RELEASED;

    while(1)
    {
        if(switchState(1) == PRESSED) ledControl(LEDGREEN, ON);
        else if(switchState(1) == RELEASED) ledControl(LEDGREEN, OFF);

        if(switchState(2) == PRESSED && PreSW2State == RELEASED)
        {
            PreSW2State = PRESSED;
            sw_press_num++;
            vTaskDelay(20);
        }
        if(switchState(2) == RELEASED && PreSW2State == PRESSED)  PreSW2State = RELEASED;
        if((sw_press_num % 2) == 0) ledControl(LEDBLUE, OFF);
        else ledControl(LEDBLUE, ON);
    }
}

static void UARTTask(void *pvParameters)
{
    while(1)
    {

    }
}
