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
uint32_t            taskredledcounter = TASKLEDREDINIT;
uint32_t            taskredledFlag = 0;

#ifdef  DEBUG
static const  char *stateName[4]={
                            "S_LEDREDON",
                              "S_WAITLEDREDOFF",
                              "S_LEDREDOFF",
                              "S_WAITLEDREDON"
                             };

#endif
void ledRedTask(void *myobject)
{
    ledRedStateMachineUpdate();
}

void ledRedStateMachineUpdate(void *myobject)
{
    switch (State)
    {
        case S_LEDOFF:
            if (switchState(1) == PRESSED)
                {
                    State = S_WAITLEDON;
                    DBG("State = %s\n",stateName[State]);
                }
            break;
        case S_WAITLEDON:
                    if (switchState(1) == RELEASED)
                        {
                            State = S_LEDON;
                            DBG("State = %s\n",stateName[State]);
                        }
                    break;
        case S_LEDON:
            if (switchState(1) == PRESSED)
                {
                    State = S_WAITLEDOFF;
                    DBG("State = %s\n",stateName[State]);
                }
            break;
        case S_WAITLEDOFF:
                    if (switchState(1) == RELEASED)
                        {
                            State = S_LEDOFF;
                            DBG("State = %s\n",stateName[State]);
                        }
                    break;
    }
    switch (State)
    {
    case S_LEDOFF:
    case    S_WAITLEDOFF:
        ledControl(LEDRED,OFF);
        break;
    case S_LEDON:
    case    S_WAITLEDON:
        ledControl(LEDRED,ON);
    }
}
