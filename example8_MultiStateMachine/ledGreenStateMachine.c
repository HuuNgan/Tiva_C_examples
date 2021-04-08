/*
 * ledRedStateMachine.c
 *
 *  Created on: Mar 16, 2018
 *      Author: Quoc Bao
 */
#include <ledGreenStateMachine.h>


typedef     enum {S_LEDON=0,S_WAITLEDOFF, S_LEDOFF, S_WAITLEDON}  ledGState_t;
uint32_t    ledGTimer = 0;

static ledGState_t State = S_LEDOFF;

#ifdef  DEBUG
  static const  char *stateName[6]={
                            //TODO
                                      "S_LEDGREENON",
                                      "S_WAITLEDGREENOFF",
                                      "S_LEDGREENOFF",
                                      "S_WAITLEDGREENON"
                             };

#endif
void ledGreenStateMachineUpdate(void)
{
  //TODO
    switch(State)
    {
        case S_LEDOFF:
            if(switchState(2) == PRESSED)
            {
                ledGTimer = 3000;
                State = S_WAITLEDON;
            }
            break;
        case S_WAITLEDON:
            while(ledGTimer != 0 && switchState(2) == PRESSED);
            if(ledGTimer == 0)
            {
                ledControl(LEDGREEN, ON);
                State = S_LEDON;
            }
            else State = S_LEDOFF;

        case S_LEDON:
            if(switchState(2) == PRESSED)
            {
                ledGTimer = 6000;
                State = S_WAITLEDOFF;
            }
            break;
        case S_WAITLEDOFF:
            while(ledGTimer != 0 && switchState(2) == PRESSED);
            if(ledGTimer == 0)
            {
                ledControl(LEDGREEN, OFF);
                State = S_LEDOFF;
            }
            else State = S_LEDON;
    }
}
