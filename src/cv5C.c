/*
 * cv5C.c
 *
 *  Created on: 18. 10. 2016
 *      Author: Peter
 */

#include <stddef.h>
#include "stm32l1xx.h"
#include "cv5H.h"
#include <stdio.h>
#include <string.h>

void adc_init(void)
{




 GPIO_InitTypeDef GPIO_InitStructure;
 ADC_InitTypeDef ADC_InitStructure;
 /* Enable GPIO clock */
 RCC_AHBPeriphClockCmd(GPIOB, ENABLE);//Opravi� a upravi�
 /* Configure ADCx Channel 2 as analog input */
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 ;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
 GPIO_Init(GPIOA, &GPIO_InitStructure);
/* Enable the HSI oscillator */
 RCC_HSICmd(ENABLE);
/* Check that HSI oscillator is ready */
 while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);
 /* Enable ADC clock */
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
 /* Initialize ADC structure */
 ADC_StructInit(&ADC_InitStructure);
 /* ADC1 configuration */
 ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
 ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
 ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
 ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
 ADC_InitStructure.ADC_NbrOfConversion = 1;
 ADC_Init(ADC1, &ADC_InitStructure);
/* ADCx regular channel8 configuration */
 ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_96Cycles);

 /* Enable the ADC */
 init_NVIC();
 ADC_Cmd(ADC1, ENABLE);


 /* Wait until the ADC1 is ready */
 while(ADC_GetFlagStatus(ADC1, ADC_FLAG_ADONS) == RESET)
 {
 }
 /* Start ADC Software Conversion */
 ADC_SoftwareStartConv(ADC1);






}
void init_NVIC(void){
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = ADC1_IRQn; //zoznam preru�en� n�jdete v s�bore stm32l1xx.h
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);

}

uint16_t value = 0;

void  ADC1_IRQHandler(void){
    if(ADC1->SR & ADC_SR_EOC){

        value = ADC1->DR;

    }
}

void initUSART2(void)   /// usart 1
{
  USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;


  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

 // RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART2, ENABLE); // usart 1

  //choosing peripherals for selected pins
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);  // GPIO_PinSource10, GPIO_AF_USART1
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);   // GPIO_PinSource9, GPIO_AF_USART1

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;   //  GPIO_Pin_10 | GPIO_Pin_9;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    //usart configuration

    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART2, &USART_InitStructure);   // usart 1
    //configuring interrupts

      NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);


      NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;   ///USART1_IRQn
      NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
      NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
      NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
      NVIC_Init(&NVIC_InitStructure);
      //choosing which event should cause interrupt
   //   USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);   // usart 1  /////// pre adc na nacitanie hodnoty

      USART_ITConfig(USART2, USART_IT_TXE, ENABLE);   // usart 1   ////////////////// pre poslednu ulohu uprava  pre posielanie hodnot

      // Enable USART
      USART_Cmd(USART2, ENABLE);   // usart 1

}

void PutcUART2(char ch){    //PutcUART1
    USART_SendData(USART2, (uint8_t) ch);   // usart 1
        while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET); // usart 1
}


char pole[];

void Put(char pole[])
{
	int i=0;

	while(pole[i]!='\0')
	{

		USART_SendData(USART2, pole[i]);   // usart 1
					        while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET); // usart 1
		   	  i++;
	}

	USART_SendData(USART2,' ');
    while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);

}

int i = 0;
int dlzka=0;
char znak[];

void posliDoFunkcie()
{
	int value1=0;


    value1=1234;

	sprintf(znak,"%d",value1);

    dlzka=strlen(znak);

}

void (* gCallback1)(unsigned char) = 0;
void RegisterCallbackUART2(void *callback){  //usart 1
    gCallback1 = callback;
}

/*    ////// IRQ handler pre primanie dat k tomu patri nastavenie aj USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
void USART2_IRQHandler(void)  //USART1_IRQHandler
{
	uint16_t pom = 0;
	    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)   // usart 1
        {
            USART_ClearITPendingBit(USART2, USART_IT_RXNE);   // usart 1
            pom = USART_ReceiveData(USART2);    // usart 1
            if (gCallback1)
            {
                gCallback1(pom);
            }
        }
}*/



 ////// IRQ handler pre primanie dat k tomu patri nastavenie aj USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
void USART2_IRQHandler(void)  //USART1_IRQHandler
{


	if(i<dlzka)
	{
			if (USART_GetFlagStatus(USART2, USART_FLAG_TC) != RESET)
			{

					if (USART_GetFlagStatus(USART2, USART_FLAG_TXE) != RESET)
					{
						 USART_SendData(USART2,znak[i]);
						 USART_ClearITPendingBit(USART2, USART_FLAG_TXE);
						 i++;
						 USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET;
					}
			}
	}
}

double pom10=0;

double prevod()
{


	 pom10=value;
	return  pom10*=0.000805;

}

int pom8=0;
double pom9 = 0;

void stav(uint16_t hodnota){




	switch (hodnota){
	case 'm' :
		if(pom8==0)
		{
			pom9 = prevod();



			uint8_t num1= (uint8_t)pom9;
			sprintf(pole,"%d.%dV", num1, (uint8_t)((pom9-num1)*100));


			Put(pole);
			pom8++;
		}
		else
		{
		  //PutcUART2(value);
			sprintf(pole,"%d",value);
			Put(pole);
			pom8=0;
		}

		break;

		}

}
