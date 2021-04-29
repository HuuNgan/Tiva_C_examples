/*
 * QEI.c
 *
 *  Created on: 29 thg 4, 2021
 *      Author: huungan
 */

#include "QEI.h"

void Config_Qei(void)
{
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_QEI0);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);

    //Unlock PD7
    HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTD_BASE + GPIO_O_CR) = 0x80;
    HWREG(GPIO_PORTD_BASE + GPIO_O_AFSEL) &= ~0x80;

    ROM_QEIConfigure(QEI0_BASE, QEI_CONFIG_CAPTURE_A_B | QEI_CONFIG_NO_RESET | QEI_CONFIG_QUADRATURE | QEI_CONFIG_NO_SWAP, 0XFFFFFFFF);
    ROM_GPIOPinTypeQEI(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7);

    ROM_GPIOPinConfigure(GPIO_PD6_PHA0);        //PF0, PD6
    ROM_GPIOPinConfigure(GPIO_PD7_PHB0);        //PF1, PD7
    ROM_QEIEnable(QEI0_BASE);

    //Configure and enable velocity
    ROM_QEIVelocityConfigure(QEI0_BASE, QEI_VELDIV_1, SysCtlClockGet()); // Divide by clock speed to get counts/sec
    ROM_QEIVelocityEnable(QEI0_BASE);
}

