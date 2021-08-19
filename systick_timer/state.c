/*
 * state.c
 *
 *  Created on: 15 thg 8, 2021
 *      Author: HuuNgan
 */
#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "state.h"

typedef     enum {S_ON1=0, S_WAITON1, S_OFF1, S_WAITOFF1,
                  S_ON2, S_WAITON2, S_OFF2, S_WAITOFF2,
                  S_ON3, S_WAITON3, S_OFF3, S_WAITOFF3}  ledRState_t;

static ledRState_t State = S_ON1;
extern uint8_t MyTimerCount;

void LEDON(void)
{
    GPIOPinWrite(LED_GPIO_BASE,GPIO_PIN_1, GPIO_PIN_1);
}

void LEDOFF(void)
{
    GPIOPinWrite(LED_GPIO_BASE,GPIO_PIN_1, 0);
}

void stateMachineUpdate(void)
{
    switch(State)
    {
        case S_WAITON1:
            MyTimerCount=1;
            State = S_ON1;
            break;
        case S_WAITOFF1:
            MyTimerCount=1;
            State = S_OFF1;
            break;
        case S_WAITON2:
            MyTimerCount=2;
            State = S_ON2;
            break;
        case S_WAITOFF2:
            MyTimerCount=2;
            State = S_OFF2;
            break;
        case S_WAITON3:
            MyTimerCount=3;
            State = S_ON3;
            break;
        case S_WAITOFF3:
            MyTimerCount=3;
            State = S_OFF3;
            break;
        default:
            ;       //do nothing
    }

    switch(State)
    {
        case S_ON1:
            if(MyTimerCount > 0) LEDON();
            else State = S_WAITOFF1;
            break;
        case S_OFF1:
            if(MyTimerCount > 0) LEDOFF();
            else State = S_WAITON2;
            break;
        case S_ON2:
            if(MyTimerCount > 0) LEDON();
            else State = S_WAITOFF2;
            break;
        case S_OFF2:
            if(MyTimerCount > 0) LEDOFF();
            else State = S_WAITON3;
            break;
        case S_ON3:
            if(MyTimerCount > 0) LEDON();
            else State = S_WAITOFF3;
            break;
        case S_OFF3:
            if(MyTimerCount > 0) LEDOFF();
            else State = S_WAITON1;
            break;
        default:
            ;       //do nothing
    }
}



