/*
 * cv5H.h
 *
 *  Created on: 18. 10. 2016
 *      Author: Peter
 */

#ifndef CV5H_H_
#define CV5H_H_

void adc_init(void);
void init_NVIC(void);
void ADC1_IRQHandler(void);
void initUSART1(void);
void PutcUART2(char ch);


#endif /* CV5H_H_ */
