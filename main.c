/*
 * main.c
 */

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "driverlib/timer.h"
#include "usingDH11.h"
#include "esp8266.h"
#include "rainCheck.h"
#include "adcControl.h"

volatile bool wifiSendFlag = true;
volatile bool adcTrigFlag = true;

uint32_t timeCount,secCount,minCount;

void LED_Init(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0x0);
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0x0);
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0x0);
}



void Timer1_Config(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
	TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);
	TimerLoadSet(TIMER1_BASE, TIMER_A, SysCtlClockGet()/100);     //10 ms 记一次
	IntPrioritySet(INT_TIMER1A ,0x00); //b '000'0 0000 //工程中的最高优先级
	IntEnable(INT_TIMER1A);
	TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
	TimerEnable(TIMER1_BASE, TIMER_A);
}


void timeInit(void)
{
	timeCount = 0;
	secCount = 0;
	minCount = 0;
}


void Timer1IntHandler(void)   //10ms执行一次
{
    TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
    timeCount++;
    if(timeCount >= 100)
    {
    	timeCount = 0;
    	secCount ++;
    	if(secCount >= 60)           //1min一次
    	{
    		secCount =0;
    		wifiSendFlag = true;
    		adcTrigFlag = true;
//    		minCount ++;
//    		if(minCount >= 60)
//    			minCount = 0;
    	}
    }
    if(rainCheck())       // no rain
    {
    	rainFlag = 0;
    	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0x0);

    }
    else                // rain
    {
    	rainFlag = 1;
    	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1);
    }
}



int main(void)
{

	SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ |
	                  SYSCTL_OSC_MAIN);//200M/4
	ConfigureUART0();
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);

    ADC_Init();
    IntMasterEnable();

	LED_Init();
	rainCheck_Init();
	esp8266_Init();
	Timer1_Config();



	while(1)
	{
		DHT11_Read_Data(&temperature,&humidity);
		if(wifiSendFlag == true)
		{
			esp8266_Connet();
			timeInit();
			wifiSendFlag = false;
		}

//		Delay_ms(2000);

	}

}










