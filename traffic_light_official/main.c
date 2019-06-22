#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#define RED_TIME  9
#define YELLOW_TIME 3
#define GREEN_TIME  7

#define RED_LIGHT   2
#define YELLOW_LIGHT  4
#define GREEN_LIGHT   8

#define RED_STATE       0
#define YELLOW_STATE    1
#define GREEN_STATE     2
uint8_t led_array[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67};
volatile uint8_t state = 0;
volatile uint8_t time = 0;
int main(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    GPIO_PORTF_DIR_R = 0x0E;
    GPIO_PORTF_DEN_R = 0x0E;
    GPIO_PORTD_DIR_R = 0x0F;
    GPIO_PORTD_DEN_R = 0x0F;
    GPIO_PORTE_DIR_R = 0x0E;
    GPIO_PORTE_DEN_R = 0x0E;
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    TimerDisable(TIMER0_BASE, TIMER_A);
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet());
    IntEnable(INT_TIMER0A);
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    TimerEnable(TIMER0_BASE, TIMER_A);
    IntMasterEnable();
    while(1)
    {
    }
    return 0;
}

void Timer0IntHandler(void)
{

    TimerIntClear(TIMER2_BASE, TIMER_TIMA_TIMEOUT);
    if(RED_STATE == state)
    {
        if(time > 0)
        {
            time--;
        }
        else
        {
            GPIO_PORTF_DATA_R = 0x08;
            state = 3;
            time = 7;
        }
    }
    else if(YELLOW_STATE == state)
    {
        if(time > 0)
        {
            time--;
        }
        else
        {
            GPIO_PORTF_DATA_R = 0x02;
            state = 0;
            time = 9;
        }
    }
    else if(GREEN_STATE == state)
    {
        if(time > 0)
        {
            time--;
        }
        else
        {
            GPIO_PORTF_DATA_R = 0x04;
            state = 1;
            time =  3;
        }
    }
    else{
        state = 0;
    }
    GPIO_PORTD_DATA_R = ~(led_array[time] & (0x0F));
    GPIO_PORTE_DATA_R = ~((led_array[time] & (0xF0))>>3);
}
