#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include <I2C/mpu6050.h>

int16_t accaxisX, accaxisY, accaxisZ, gyroaxisX, gyroaxisY, gyroaxisZ;
float x_degree;

float convertToDegree(int16_t accelValue)
{
    return (float)accelValue*90/MPU6050_ACC_SCALE_FACTOR_8;
}

float gyroAngle(int16_t gyroValue)
{
#define time 0.02
    float gyroAngle = 0;
    float gyroRate = gyroaxisX*90/MPU6050_GYRO_SCALE_FACTOR_2000;
    gyroAngle += gyroRate*time
}

int main(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
    initI2C();
    initMPU6050();
    while(1)
    {
        getMPU6050Data();
        x_degree = convertToDegree(accaxisX);
        SysCtlDelay(1000000);
    }
}
