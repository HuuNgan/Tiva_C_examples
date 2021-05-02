#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/tm4c123gh6pm.h"
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

#define POSITION_ERR    5
#define DOOR_CLOSE_POS  1000
#define DOOR_OPEN_POS   1374
#define DOOR_OPEN_TIME  5   //seconds

enum {CLOSED, OPENED, WAITING_OPEN, WAITING_CLOSE} DoorState;

typedef struct
{
    volatile int speed, direction;
    volatile uint16_t position;
    uint16_t stopPosition;
}Encoder;

typedef struct
{
    float Kp, Ki, Kd, current_err, previous_err;
    float iteration_sum, iteration_time;
    uint16_t positionSet;
}PID_coefficient;

Encoder encoderA, encoderB;
PID_coefficient PID_B={0.2, 0.000015, 10};
int16_t dutyB;
uint8_t timerCount=0;

void GPIOInterruptInit(){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    //Unlock the GPIO Lock for PF0
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) = 0x01;
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;

//    GPIOIntRegister(GPIO_PORTD_BASE);
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0);
    GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_0, GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);  //Pullup
    GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_FALLING_EDGE);

    GPIOIntEnable(GPIO_PORTF_BASE, GPIO_PIN_0);
    GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_0);
    IntMasterEnable();
    IntEnable(INT_GPIOF);
    //  HWREG(0x40004410) = 0xFF;
}

void timerInit(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC_UP);
    TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet()-1);

    IntEnable(INT_TIMER0A);
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    IntMasterEnable();

    TimerEnable(TIMER0_BASE, TIMER_A);
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
    return (float)time_duration/(float)(SysCtlClockGet()/1000);      //return in ms
}

void setMotorB_Position(void)
{
    encoderB.position = ROM_QEIPositionGet(QEI0_BASE);
    PID_B.current_err = PID_B.positionSet - encoderB.position;

    while(abs(PID_B.current_err) > POSITION_ERR)
    {
        PID_B.iteration_time = getTimeValue();
        encoderB.position = ROM_QEIPositionGet(QEI0_BASE);
        if(encoderB.position > PID_B.positionSet)    motorB_run(backward);
        else if(encoderB.position < PID_B.positionSet)    motorB_run(forward);
        PID_B.current_err = PID_B.positionSet - encoderB.position;
        PID_B.iteration_sum += PID_B.current_err*PID_B.iteration_time;
        dutyB =PID_B.Kp*PID_B.current_err + PID_B.Ki*PID_B.iteration_sum
                + PID_B.Kd*(PID_B.current_err-PID_B.previous_err)/PID_B.iteration_time;
        if(dutyB >= 0)  setSpeed_B(dutyB+150);
        else    setSpeed_B(-dutyB+150);
        PID_B.previous_err = PID_B.current_err;
    }
    release_B();
    PID_B.iteration_sum = 0;    //reset value
}

void UpdateStateMachine()
{
    switch(DoorState)
    {
        case WAITING_OPEN:
            PID_B.positionSet = DOOR_OPEN_POS;
            setMotorB_Position();
            DoorState = OPENED;
            timerCount = 0;
            break;
        case OPENED:
            if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0) != GPIO_PIN_0)  timerCount = 0;
            if(timerCount >= DOOR_OPEN_TIME)
                DoorState = WAITING_CLOSE;
            break;
        case WAITING_CLOSE:
            PID_B.positionSet = DOOR_CLOSE_POS;
            setMotorB_Position();
            DoorState = CLOSED;
            break;
    }
}

void Timer0AIntHandler(void)
{
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    timerCount++;
}

void GPIOFIntHandler(void)
{
    GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_0);

    if(DoorState == CLOSED || DoorState == WAITING_CLOSE)
    {
        DoorState = WAITING_OPEN;
    }
    else if(DoorState == OPENED)    timerCount = 0;     //reset timer
}

int main(void)
{
    //Initialize clock
    SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

    PWM_Config();
    MOTOR_Config();
    Config_Qei();
    timerInit();
    GPIOInterruptInit();

    QEIPositionSet(QEI0_BASE, DOOR_CLOSE_POS);
    PID_B.positionSet = DOOR_CLOSE_POS;
    DoorState = CLOSED;

    while(1)
    {
        UpdateStateMachine();
//        setMotorB_Position();
//        test = dutyB*0.6+400;
//        setSpeed_B(dutyB);
//        motorB_run(forward);
    }
//	return 0;
}
