/*
 * state.h
 *
 *  Created on: 15 thg 8, 2021
 *      Author: HuuNgan
 */

#ifndef STATE_H_
#define STATE_H_

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_gpio.h"
#include "driverlib/gpio.h"
#define TARGET_IS_BLIZZARD_RB1
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"

#define LED_GPIO_PERIPH     SYSCTL_PERIPH_GPIOF
#define LED_GPIO_BASE       GPIO_PORTF_BASE


enum ledNumber {LEDRED = 0, LEDBLUE, LEDGREEN};
enum ledState  {OFF=0,ON=1};
void ledInit(void);
void LEDON(void);
void LEDOFF(void);
void stateMachineUpdate(void);


#endif /* STATE_H_ */
