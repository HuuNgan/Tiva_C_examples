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
#include "driverlib/interrupt.h"
#include "Motor/motor.h"
#include "QEI/QEI.h"

uint32_t position;
int32_t direction;
uint16_t speed;


int main(void)
{
    //Initialize clock (80MHZ)
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
    Config_Qei();
    IntMasterEnable();

    while(1)
    {
        ROM_SysCtlDelay(ROM_SysCtlClockGet()/3);
        position = ROM_QEIPositionGet(QEI0_BASE);
        speed = ROM_QEIVelocityGet(QEI0_BASE);
        direction = ROM_QEIDirectionGet(QEI0_BASE);
    }

//	return 0;
}
