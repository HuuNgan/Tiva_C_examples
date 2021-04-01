#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include <I2C/mpu6050.h>

uint8_t accaxisX, accaxisY, accaxisZ, gyroaxisX, gyroaxisY, gyroaxisZ;

int main(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
    initI2C();
    initMPU6050();
    while(1)
    {
        getMPU6050Data();
        SysCtlDelay(1000000);
    }
}
