/*
 * rainCheck.c
 *
 *  Created on: 2019Äê2ÔÂ26ÈÕ
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
#include "adcControl.h"


uint32_t rainFlag;

void rainCheck_Init(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB));
	GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_4);
	GPIOPadConfigSet(GPIO_PORTB_BASE,GPIO_PIN_4,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
}

uint8_t rainCheck(void)
{
	if(GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_4)&GPIO_PIN_4)
		return 1;            //   no rain
	else
		return 0;            //   rain
}
