/*
 * QEI.h
 *
 *  Created on: 29 thg 4, 2021
 *      Author: huungan
 */

#ifndef QEI_QEI_H_
#define QEI_QEI_H_

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/debug.h"
#include "driverlib/pwm.h"
#include "driverlib/pin_map.h"
#include "driverlib/qei.h"
#include "driverlib/rom.h"
#include "Motor/motor.h"

extern void Config_Qei(void);

#endif /* QEI_QEI_H_ */
