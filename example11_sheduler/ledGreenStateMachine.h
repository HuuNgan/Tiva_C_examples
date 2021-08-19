/*
 * ledRedStateMachine.h
 *
 *  Created on: Mar 16, 2018
 *      Author: Quoc Bao
 */

#ifndef LEDGREENSTATEMACHINE_H_
#define LEDGREENSTATEMACHINE_H_

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "switches.h"
#include "led.h"
#include "uartstdio.h"
#include "debug.h"
extern uint32_t    ledGTimer;

#define     TASKLEDGREENPERIOD    50      //period is 50 ms
#define     TASKLEDGREENINIT      1    //start after 1 ms
extern      uint32_t            taskgreenledcounter;
extern      uint32_t            taskgreenledFlag;

void ledGreenTask(void *myobject);

#endif /* LEDGREENSTATEMACHINE_H_ */
