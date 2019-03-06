/*
 * adcControl.h
 *
 *  Created on: 2018Äê7ÔÂ17ÈÕ
 *      Author: CZW
 */

#ifndef ADCCONTROL_H_
#define ADCCONTROL_H_

extern volatile bool  ADCsend;
//extern volatile bool ADCflag;
extern volatile bool  bDataReady;     // The flag indicates is data ready.
extern uint32_t X_Ch0Value;          // Savve the voltage value of ch0 and ch1.
//extern uint32_t Sign;

extern void ADC_Init(void);
extern void ADC0IntHander(void);
extern void ADC_Trig(void);




#endif /* ADCCONTROL_H_ */
