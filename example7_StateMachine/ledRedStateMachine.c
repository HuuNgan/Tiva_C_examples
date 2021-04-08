/*
 * ledRedStateMachine.c
 *
 *  Created on: Mar 16, 2018
 *      Author: Quoc Bao
 */
#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "ledRedStateMachine.h"
#include "driverlib/gpio.h"
#include "switches.h"
#include "led.h"
#include "uartstdio.h"
#include "debug.h"

typedef     enum {S_LEDON=0,S_WAITLEDOFF, S_LEDOFF, S_WAITLEDON}  ledRState_t;

static ledRState_t State = S_LEDOFF;

#ifdef  DEBUG
  const  char *stateName[4]={
                            "S_LEDREDON",
                              "S_WAITLEDREDOFF",
                              "S_LEDREDOFF",
                              "S_WAITLEDREDON"
                             };

#endif
void ledRedStateMachineUpdate(void)
{
    //TODO
//    if(switchState(1) == PRESSED && State == S_LEDOFF)
//    {
//        State = S_WAITLEDON;
//        ledControl(LEDRED, ON);
//        State = S_LEDON;
//    }
//    else if(switchState(1) == PRESSED && State == S_LEDON)
//    {
//        State = S_WAITLEDOFF;
//        ledControl(LEDRED, OFF);
//        State = S_LEDOFF;
//    }
    switch(State)
    {
        case S_LEDOFF:
            while(switchState(1) == PRESSED)
            {
                State = S_WAITLEDON;
                ledControl(LEDRED, ON);
//                SysCtlDelay(500000);      //debounce
                State = S_LEDON;
            }
            break;
        case S_LEDON:
            while(switchState(1) == PRESSED)
            {
                State = S_WAITLEDOFF;
                ledControl(LEDRED, OFF);
//                SysCtlDelay(500000);      //debounce
                State = S_LEDOFF;
            }
            break;
        default:
            ;       //do nothing
    }
}
