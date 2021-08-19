/*
 * ledRedStateMachine.h
 *
 *  Created on: Mar 16, 2018
 *      Author: Quoc Bao
 */

#ifndef LEDREDSTATEMACHINE_H_
#define LEDREDSTATEMACHINE_H_

#define     TASKLEDREDPERIOD    50      //period is 50 ms
#define     TASKLEDREDINIT      0    //start immediately
extern      uint32_t            taskredledcounter;
extern      uint32_t            taskredledFlag;

void ledRedTask(void *myobject);

#endif /* LEDREDSTATEMACHINE_H_ */
