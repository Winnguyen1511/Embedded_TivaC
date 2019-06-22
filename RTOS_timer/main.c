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
#define RED_TIME  7
#define YELLOW_TIME 2
#define GREEN_TIME  5

#define RED_LIGHT   2
#define YELLOW_LIGHT  4
#define GREEN_LIGHT   8

#define RED_PIN     GPIO_PIN_1
#define YELLOW_PIN  GPIO_PIN_2
#define GREEN_PIN   GPIO_PIN_3

#define RED_STATE       1
#define YELLOW_STATE    2
#define GREEN_STATE     0

uint8_t state = 0;
uint8_t time_count = 0;
/*
 *  ======== taskFxn ========
 */
Void taskFxn(UArg a0, UArg a1)
{
    //System_printf("enter taskFxn()\n");
	
    Task_sleep(10);
	
    //System_printf("exit taskFxn()\n");
}
void configClocks(void);
void configTimer(void);
void configIO(void);
/*
 *  ======== main ========
 */
Void Idle_function(Void)
{

}
Void LED_ISR(Void)
{
    if(RED_STATE == state)
    {
        GPIO_PORTF_DATA_R = GREEN_LIGHT;
        state = GREEN_STATE;
        time_count = GREEN_TIME;
    }
    else if(YELLOW_STATE == state)
    {
        GPIO_PORTF_DATA_R = RED_LIGHT;
        state = RED_STATE;
        time_count = RED_TIME;
    }
    else{
        GPIO_PORTF_DATA_R = YELLOW_LIGHT;
        state = YELLOW_STATE;
        time_count =  YELLOW_TIME;
    }
}
Void Timer2A_ISR(Void)
{
    TimerIntClear(TIMER2_BASE, TIMER_TIMA_TIMEOUT);
    if(time_count >0)
        time_count--;
    else
    {
        Swi_post(TimerSwi);
    }
}
Int main()
{ 
	configClocks();
	configIO();
	configTimer();
	TimerEnable(TIMER2_BASE, TIMER_A);
	Swi_enable();
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
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
    //GPIOPinTypeGPIOInput()
}
void configTimer(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);
    TimerConfigure(TIMER2_BASE, TIMER_CFG_PERIODIC);
    uint32_t ui32Period = SysCtlClockGet();
    TimerLoadSet(TIMER2_BASE, TIMER_A, ui32Period);
    IntEnable(INT_TIMER2A);
    TimerIntEnable(TIMER2_BASE, TIMER_TIMA_TIMEOUT);
}
