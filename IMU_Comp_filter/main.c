#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/tm4c123gh6pm.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "debug.h"
#include <I2C/mpu6050.h>

#define     CALIB
//#define     DEBUG_UART

int16_t accaxisX, accaxisY, accaxisZ, gyroaxisX, gyroaxisY, gyroaxisZ;
float acc_angle_x;
float gyroAngle_x = 0;
float gyroRate;
float filted_Angle;

typedef struct
{
    int16_t acc_x, acc_y, acc_z;
    int16_t gyro_x, gyro_y, gyro_z;
} OffsetCalibration;

OffsetCalibration OFFSET = {62, 103, 32, 989, 8, 3740};

//**********DEBUG**************************************************************
#ifdef DEBUG_UART
void InitConsole(void)
{
    //
    // Enable GPIO port A which is used for UART0 pins.
    // TODO: change this to whichever GPIO port you are using.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    //
    // Configure the pin muxing for UART0 functions on port A0 and A1.
    // This step is not necessary if your part does not support pin muxing.
    // TODO: change this to select the port/pin you are using.
    //
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);

    //
    // Enable UART0 so that we can configure the clock.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    //
    // Use the internal 16MHz oscillator as the UART clock source.
    //
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);

    //
    // Select the alternate (UART) function for these pins.
    // TODO: change this to select the port/pin you are using.
    //
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    //
    // Initialize the UART for console I/O.
    //
    UARTStdioConfig(0, 115200, 16000000);
}
#endif
//*****************************************************************************

//**********Calibration mode***************************************************
#ifdef CALIB
#define NUM_OF_SAMPLES          10

void Calib_IMU(void)
{
    int16_t buffer[NUM_OF_SAMPLES];
    uint8_t i;
    int32_t sum;

    SysCtlDelay(SysCtlClockGet()/6);
    for(i=0; i<NUM_OF_SAMPLES; i++)
    {
        getMPU6050Data();
        buffer[i] = accaxisX;
        SysCtlDelay(SysCtlClockGet()/150);
    }
    sum=0;
    for(i=0; i<NUM_OF_SAMPLES; i++) sum += buffer[i];
    OFFSET.acc_x = sum/NUM_OF_SAMPLES;

//    for(i=0; i<NUM_OF_SAMPLES; i++)
//    {
//        getMPU6050Data();
//        buffer[i] = accaxisY;
//        SysCtlDelay(SysCtlClockGet()/150);
//    }
//    sum=0;
//    for(i=0; i<NUM_OF_SAMPLES; i++) sum += buffer[i];
//    OFFSET.acc_y = sum/NUM_OF_SAMPLES;
//
//    for(i=0; i<NUM_OF_SAMPLES; i++)
//    {
//        getMPU6050Data();
//        buffer[i] = accaxisZ;
//        SysCtlDelay(SysCtlClockGet()/150);
//    }
//    sum=0;
//    for(i=0; i<NUM_OF_SAMPLES; i++) sum += buffer[i];
//    OFFSET.acc_z = sum/NUM_OF_SAMPLES;
//
//    for(i=0; i<NUM_OF_SAMPLES; i++)
//    {
//        getMPU6050Data();
//        buffer[i] = gyroaxisX;
//        SysCtlDelay(SysCtlClockGet()/150);
//    }
//    sum=0;
//    for(i=0; i<NUM_OF_SAMPLES; i++) sum += buffer[i];
//    OFFSET.gyro_x = sum/NUM_OF_SAMPLES;

    for(i=0; i<NUM_OF_SAMPLES; i++)
    {
        getMPU6050Data();
        buffer[i] = gyroaxisY;
        SysCtlDelay(SysCtlClockGet()/150);
    }
    sum=0;
    for(i=0; i<NUM_OF_SAMPLES; i++) sum += buffer[i];
    OFFSET.gyro_y = sum/NUM_OF_SAMPLES;
//
//    for(i=0; i<NUM_OF_SAMPLES; i++)
//    {
//        getMPU6050Data();
//        buffer[i] = gyroaxisZ;
//        SysCtlDelay(SysCtlClockGet()/150);
//    }
//    sum=0;
//    for(i=0; i<NUM_OF_SAMPLES; i++) sum += buffer[i];
//    OFFSET.gyro_z = sum/NUM_OF_SAMPLES;
}
#endif
//*****************************************************************************

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
    getMPU6050Data();
    gyroRate = -(float)(gyroaxisY - OFFSET.gyro_y)/MPU6050_GYRO_SCALE_FACTOR_2000;
    gyroAngle_x += gyroRate*0.02f;
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
}

int main(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

    #ifdef  DEBUG_UART
        InitConsole();
        DBG("\nDebug is ON\n\n");
    #endif

        initI2C();
        initMPU6050();
        timerInit();
    #ifdef CALIB
        Calib_IMU();
    #endif

    #ifdef DEBUG_UART
        DBG("acc_x_offset = %d\n", (int)OFFSET.acc_x);
        DBG("acc_y_offset = %d\n", (int)OFFSET.acc_y);
        DBG("acc_z_offset = %d\n", (int)OFFSET.acc_z);
        DBG("gyro_x_offset = %d\n", (int)OFFSET.gyro_x);
        DBG("gyro_y_offset = %d\n", (int)OFFSET.gyro_y);
        DBG("gyro_z_offset = %d\n", (int)OFFSET.gyro_z);
    #endif

    while(1)
    {
//        getMPU6050Data();
        acc_angle_x = convertToDegree(accaxisX - OFFSET.acc_x);
        filted_Angle = Comp_Filter(gyroAngle_x, acc_angle_x);

        #ifdef DEBUG_UART
            UARTprintf("acc_angle_x = %d\n", (int)acc_angle_x);
            UARTprintf("gyro_angle_x = %d\n", (int)gyroAngle_x);
            UARTprintf("Comp_Angle_x = %d\n", (int)filted_Angle);
        #endif
        SysCtlDelay(SysCtlClockGet()/6);
    }
}
