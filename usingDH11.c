/*
 * usingDH11.c
 *
 *  Created on: 2019年2月24日
 *      Author: CZW
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

#define DHT11_BASE      		GPIO_PORTA_BASE
#define DHT11_PIN           	GPIO_PIN_7
#define SYSCTL_PERIPH_DHT11  	SYSCTL_PERIPH_GPIOA


volatile bool DHT11Update_Flag = false;
uint8_t temperature,humidity;


uint8_t DHT11_Init(void);
void DHT11_Rst(void);
void DHT11_Output(void);
void DHT11_Iutput(void);
uint8_t DHT11_Check(void);
uint8_t DHT11_Read_Data();
uint8_t DHT11_Read_Byte(void);
uint8_t DHT11_Read_Bit(void);


void Delay_ms(uint32_t xms)
{
	SysCtlDelay(xms*(SysCtlClockGet()/3000));
}
void Delay_us(uint32_t xus)
{
	SysCtlDelay(xus*(SysCtlClockGet()/3000000));
}


uint8_t DHT11_DQ_IN(void)
{
	if(GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_7)&GPIO_PIN_7)
		return 1;
	else
		return 0;
}

void DHT11_Output(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_DHT11);
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_DHT11));
	GPIOPinTypeGPIOOutput(DHT11_BASE, DHT11_PIN);
}




void DHT11_Iutput(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_DHT11);
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_DHT11));
	GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, GPIO_PIN_7);
	GPIOPadConfigSet(GPIO_PORTA_BASE,GPIO_PIN_7,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
}

void DHT11_Rst(void)
{
	DHT11_Output(); 	 //设置输出
    GPIOPinWrite(DHT11_BASE, DHT11_PIN, 0x0);
	Delay_ms(20);    		 //至少18ms拉下来
	GPIOPinWrite(DHT11_BASE, DHT11_PIN, DHT11_PIN);
	Delay_us(30);     	         //上拉20～40us
}


uint8_t DHT11_Check(void)
{
	uint8_t retry=0;
	DHT11_Iutput();//SET INPUT
    while (DHT11_DQ_IN()&&retry<100)//
	{
		retry++;
		Delay_us(1);
	}
	if(retry>=100)return 1;
	else retry=0;
    while (!DHT11_DQ_IN()&&retry<100)//
    {
		retry++;
		Delay_us(1);
	}
	if(retry>=100) return 1;
	return 0;
}


//**********************handle data


uint8_t DHT11_Read_Data(uint8_t *temp,uint8_t *humi)
{
 	uint8_t buf[5];
	uint8_t i;
	DHT11_Rst();
	if(DHT11_Check()==0)
	{
		for(i=0;i<5;i++)//
		{
			buf[i]=DHT11_Read_Byte();
		}
		if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4])
		{
			*humi=buf[0];
			*temp=buf[2];
		}
//		UARTprintf("temperature :%d,humidity :%d\n",temperature,humidity);
	}else return 1;
	return 0;
}


uint8_t DHT11_Read_Byte(void)
{
    uint8_t i,dat;
    dat=0;
	for (i=0;i<8;i++)
	{
   		dat<<=1;
	    dat|=DHT11_Read_Bit();
    }
    return dat;
}


uint8_t DHT11_Read_Bit(void)
{
 	uint8_t retry=0;
	while(DHT11_DQ_IN() && retry<100)//
	{
		retry++;
		Delay_us(1);
	}
	retry=0;
	while(!DHT11_DQ_IN() && retry<100)//
	{
		retry++;
		Delay_us(1);
	}
	Delay_us(40);//
	if(DHT11_DQ_IN())return 1;
	else return 0;
}



uint8_t DHT11_Init(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_DHT11);
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_DHT11));
	GPIOPinTypeGPIOOutput(DHT11_BASE, DHT11_PIN);

	DHT11_Rst();
	return DHT11_Check();
}

//******************************************








