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

void initUSART2(void);  // usart 1
void PutcUART2(char);// usart 1

void RegisterCallbackUART2(void *callback);   // usart 1
void USART2_IRQHandler(void); // usart 1

double prevod();
void stav(uint16_t);
void Put(char []);
void posliPoZnaku(char , int );
void posliDoFunkcie();

char znak[];

#endif /* CV5H_H_ */
