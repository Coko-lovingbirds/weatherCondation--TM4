/*
 * esp8266.c
 *
 *  Created on: 2019年2月26日
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
#include "esp8266.h"
#include "rainCheck.h"

void ConfigureUART0(void)
{

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);
    UARTStdioConfig(0, 115200, 16000000);
}


void esp8266_Init(void)
{
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0x0);

	UARTprintf("AT+RST\r\n");    ////恢复到初始状态
	Delay_ms(10000);

	UARTprintf("AT+CWMODE=3\r\n");  //设置WiFi应用模式为Station和Server共存模式
	Delay_ms(8000);

	UARTprintf("AT+CWJAP=\"Honor8\",\"56781234dzf\"\r\n");   //连接路由器  Honer8为路由器用户名，12345678为密码
	Delay_ms(8000);

	UARTprintf("AT+CIPSTART=\"TCP\",\"found.nymrli.top\",8888\r\n");  //need change  //建立远程TCP连接
	Delay_ms(10000);

	UARTprintf("AT+CIPMODE=1\r\n");
	Delay_ms(8000);

	UARTprintf("AT+CIPSEND\r\n");
	Delay_ms(2000);




}

void esp8266_Connet(void)
{
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3);
	UARTprintf("POST /api/update/mcu HTTP/1.1\n");
	UARTprintf("Host:found.nymrli.top:8888\n");
	UARTprintf("Content-Type: application/x-www-form-urlencoded\n");
	UARTprintf("Connection:close\n");
	UARTprintf("Content-Length:32\n");
	UARTprintf("\n");
	//	UARTprintf("{\"data\":[{\"temperature\":5},{\"humidity\":7},{\"wet\":8}]}\r\n");
	UARTprintf("temperature=%d&humidity=%d&wet=%d\n",temperature,humidity,rainFlag);

}

void esp8266ConnetTest(void)
{
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3);
	UARTprintf("POST /post HTTP/1.1\n");
	UARTprintf("Host:httpbin.org\n");
	UARTprintf("Content-Type: application/x-www-form-urlencoded\n");
	UARTprintf("Connection:close\n");
	UARTprintf("Content-Length:32\n");
	UARTprintf("\n");
	//	UARTprintf("{\"data\":[{\"temperature\":5},{\"humidity\":7},{\"wet\":8}]}\r\n");
	UARTprintf("temperature=%d&humidity=%d&wet=%d\n",temperature,humidity,rainFlag);

}


/*
 *
POST /post HTTP/1.1
Host:httpbin.org
Content-Type: application/x-www-form-urlencoded
Connection:close
Content-Length:32

temperature=22&humidity=22&wet=1
*/


/*
POST /api/update/mcu HTTP/1.1
Host:found.nymrli.top:8888
Content-Type: application/x-www-form-urlencoded
Connection:close
Content-Length:32

temperature=22&humidity=22&wet=1
*/

