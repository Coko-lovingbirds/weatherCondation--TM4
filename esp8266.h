/*
 * esp8266.h
 *
 *  Created on: 2019��2��26��
 *      Author: CZW
 */

#ifndef ESP8266_H_
#define ESP8266_H_

extern uint32_t rainFlag;

extern void ConfigureUART0(void);
extern void esp8266_Init(void);
extern void esp8266_Connet(void);
extern void esp8266ConnetTest(void);

#endif /* ESP8266_H_ */
