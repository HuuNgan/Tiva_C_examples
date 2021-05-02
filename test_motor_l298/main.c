//*****************************************************************************
//----------------------------DC MOTOR CONTROL---------------------------------
//--------------------------Pay It Forward Club--------------------------------
//-------------------------Author: LUONG HUU NGAN------------------------------
//-------------------------Created on: May 7, 2019-----------------------------
//-------------------------Feel Free to copy it--------------------------------
//*****************************************************************************
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/debug.h"
#include "driverlib/pwm.h"
#include "driverlib/pin_map.h"
#include "driverlib/systick.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#include "inc/hw_gpio.h"
#include "Motor/motor.h"
#include "QEI/QEI.h"

enum {Release, RunA, ENCODER, PID, ENC_Position, PID_Position} Mode;
typedef struct
{
    volatile int speed, direction;
    uint16_t position, stopPosition;
}Encoder;

typedef struct
{
    float Kp, Ki, Kd, speed, current_err, previous_err;
    float iteration_sum, iteration_time;
    uint16_t positionSet;
}PID_coefficient;

float dutyA = 500, dutyB=500;
uint32_t sysTickValue;
uint8_t sysTickCount;
uint8_t timerCount;
Encoder encoderA, encoderB;
PID_coefficient PID_B={1.3, 0.000035, 0.0005, 50};

//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
    while(1);
}
#endif

//*****************************************************************************
void timerInit(void)
{
//    #define INT_TIMER0A     0x23

    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC_UP);
    TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet()-1);

//    IntEnable(INT_TIMER0A);
//    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
//    IntMasterEnable();

    TimerEnable(TIMER0_BASE, TIMER_A);
}

void SysTickConfig(void)
{
    SysTickPeriodSet(SysCtlClockGet()/10);

    // Enable interrupts to the processor.
    IntMasterEnable();
    // Enable the SysTick Interrupt.
    SysTickIntEnable();
    // Enable SysTick.
    SysTickEnable();
}

float getTimeValue(void)
{
    uint32_t time_duration, timerValue;
    static int previousTimerValue=0;
    timerValue = TimerValueGet(TIMER0_BASE, TIMER_A);
    if(timerValue >= previousTimerValue)
    {
        time_duration = timerValue - previousTimerValue;
    }
    else
    {
        time_duration = SysCtlClockGet() - previousTimerValue + timerValue;
    }
    previousTimerValue = timerValue;
    return time_duration*0.1/(SysCtlClockGet()/10);
}

void SysTickIntHandler(void)
{
    sysTickCount ++;
}

void Timer0IntHandler(void)
{
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    timerCount++;
}

int main(void)
{
    //Initialize clock
    SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

    PWM_Config();
    MOTOR_Config();
    Config_Qei();
//    SysTickConfig();
    timerInit();

    Mode = PID_Position;
    PID_B.iteration_time = SysTickValueGet();
    encoderB.stopPosition = 1495;

    //Loop forever
    while(1)
    {
        switch(Mode)
        {
            case RunA:
                motorA_run(forward);
                for(dutyA=400; dutyA<=1000; dutyA++)
                {
                    setSpeed_A(dutyA);
                    encoderA.speed = ROM_QEIVelocityGet(QEI0_BASE);
                    encoderA.direction = ROM_QEIDirectionGet(QEI0_BASE);
                    SysCtlDelay(100000);
                }
                setSpeed_A(1);
                SysCtlDelay(50000000);

                motorA_run(backward);
                for(dutyA=400; dutyA<=1000; dutyA++)
                {
                    setSpeed_A(dutyA);
                    encoderA.speed = ROM_QEIVelocityGet(QEI0_BASE);
                    encoderA.direction = ROM_QEIDirectionGet(QEI0_BASE);
                    SysCtlDelay(100000);
                }
                setSpeed_A(1);
                SysCtlDelay(50000000);
                break;
            case ENCODER:
                setSpeed_B(dutyB);
                motorB_run(forward);
                encoderB.speed = ROM_QEIVelocityGet(QEI0_BASE)*60/1496;
                encoderB.direction = ROM_QEIDirectionGet(QEI0_BASE);
                break;
            case PID:
                PID_B.iteration_time = getTimeValue();
                encoderB.direction = ROM_QEIDirectionGet(QEI0_BASE);
                encoderB.speed = ROM_QEIVelocityGet(QEI0_BASE)*60/1496;
                PID_B.current_err = PID_B.speed - encoderB.speed;
                PID_B.iteration_sum += PID_B.current_err*PID_B.iteration_time;
                dutyB = PID_B.Kp*PID_B.current_err + PID_B.Ki*PID_B.iteration_sum
                        + PID_B.Kd*(PID_B.current_err-PID_B.previous_err)/PID_B.iteration_time;
                if(dutyB<0) dutyB = 1;
                setSpeed_B(dutyB);
                motorB_run(forward);
                PID_B.previous_err = PID_B.current_err;
                break;
            case ENC_Position:
                setSpeed_B(500);
                motorB_run(forward);
                encoderB.position = ROM_QEIPositionGet(QEI0_BASE);
                while(encoderB.position < encoderB.stopPosition)
                {
                    motorB_run(forward);
                    encoderB.position = ROM_QEIPositionGet(QEI0_BASE);
                }
                release_B();
                SysCtlDelay(SysCtlClockGet()/3);
                break;
            case PID_Position:
                PID_B.iteration_time = getTimeValue();
                encoderB.position = ROM_QEIPositionGet(QEI0_BASE);
                if(encoderB.position > PID_B.positionSet)    motorB_run(backward);
                else if(encoderB.position < PID_B.positionSet)    motorB_run(forward);
                PID_B.current_err = PID_B.positionSet - encoderB.position;
                PID_B.iteration_sum += PID_B.current_err*PID_B.iteration_time;
                dutyB = PID_B.Kp*PID_B.current_err + PID_B.Ki*PID_B.iteration_sum
                        + PID_B.Kd*(PID_B.current_err-PID_B.previous_err)/PID_B.iteration_time;
                if(dutyB >= 0)  setSpeed_B(dutyB);
                else    setSpeed_B(-dutyB);
                PID_B.previous_err = PID_B.current_err;
                break;
            case Release:
                release_A();
                release_B();
                break;
        }
    }
}
