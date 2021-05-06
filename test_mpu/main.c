#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/tm4c123gh6pm.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"
#include <I2C/mpu6050.h>

int16_t accaxisX, accaxisY, accaxisZ, gyroaxisX, gyroaxisY, gyroaxisZ;
float x_degree;
//float gyro_x_degree;
float gyroAngle_x = 0;
float gyroRate;

float convertToDegree(int16_t accelValue)
{
    return (float)accelValue*90/MPU6050_ACC_SCALE_FACTOR_8;
}

void timerInit(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC_UP);
    TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet()/50-1);  //20ms

    IntEnable(INT_TIMER0A);
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    IntMasterEnable();

    TimerEnable(TIMER0_BASE, TIMER_A);
}

void Timer0AIntHandler(void)
{
    gyroRate = (float)(gyroaxisX + MPU6050_GYRO_OFFSET_X)/MPU6050_GYRO_SCALE_FACTOR_2000;
    gyroAngle_x += gyroRate*0.02f;
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
}

int main(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
    initI2C();
    initMPU6050();
    timerInit();
    while(1)
    {
        getMPU6050Data();
        x_degree = convertToDegree(accaxisX);
        SysCtlDelay(1000000);
    }
}
