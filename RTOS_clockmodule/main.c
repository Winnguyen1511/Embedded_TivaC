/*
 *  ======== main.c ========
 */

#include <xdc/std.h>                        //mandatory - have to include first, for BIOS types
#include <ti/sysbios/BIOS.h>                //mandatory - if you call APIs like BIOS_start()
#include <xdc/runtime/Log.h>                //needed for any Log_info() call
#include <xdc/cfg/global.h>                 //header file for statically defined objects/handles

#include <ti/sysbios/knl/Task.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"

/*
 *  ======== taskFxn ========
 */
Void taskFxn(UArg a0, UArg a1)
{
    //System_printf("enter taskFxn()\n");
	
    Task_sleep(10);
	
    //System_printf("exit taskFxn()\n");
}
Void ledToggle(Void)
{
    static uint32_t i16ToggleCount = 0;
    GPIO_PORTF_DATA_R ^= 0x02;
    Log_info1("LED TOGGLED [%u] TIMES",i16ToggleCount++);
}
/*
 *  ======== main ========
 */
void configClocks(void);
void configIO(void);
Int main()
{ 
    /*
     * use ROV->SysMin to view the characters in the circular buffer
     */
    //System_printf("enter main()\n");
	configClocks();
	configIO();
    BIOS_start();    /* does not return */
    return(0);
}

void configClocks(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
}
void configIO(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1);
}
