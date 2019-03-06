/*
 * usingDH11.h
 *
 *  Created on: 2019Äê2ÔÂ24ÈÕ
 *      Author: CZW
 */

#ifndef USINGDH11_H_
#define USINGDH11_H_

extern volatile bool DHT11Update_Flag;
extern uint8_t temperature,humidity;

extern void Delay_ms(uint32_t xms);
extern void Delay_us(uint32_t xus);
extern uint8_t DHT11_Init(void);
extern void DHT11_Rst(void);
extern void DHT11_Output(void);
extern void DHT11_Iutput(void);
extern uint8_t DHT11_Check(void);
extern uint8_t DHT11_Read_Data();
extern uint8_t DHT11_Read_Byte(void);
extern uint8_t DHT11_Read_Bit(void);
extern uint8_t DHT11_DQ_IN(void);

#endif /* USINGDH11_H_ */
