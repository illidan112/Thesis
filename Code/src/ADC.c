// File     : ADC.c
//--------------------------------------------------------------

//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "stm32f4xx.h"
#include "ADC.h"


//--------------------------------------------------------------
// Local
//--------------------------------------------------------------

uint16_t reads[SAMP_SIZ], sum;
long int  ptr;
uint16_t reader, last;
uint16_t mass[3];

//--------------------------------------------------------------
// Initialization ADC
// MAX Fadc = 36MHz
//--------------------------------------------------------------
void ADC_Init(void){
	int i;

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;

	GPIOC->MODER |= GPIO_MODER_MODER0;  						// PC0 - analog input
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN; 						// enable of clocking ADC (разрешение тактирование АЦП)
	ADC->CCR |= ADC_CCR_ADCPRE_0; 									// prescaler = 4 (предделитель /4)
	//ADC1->CR1 |= ADC_CR1_RES_0;
	ADC1->CR2 |= ADC_CR2_ADON; 											// enable ADC (разрешить АЦП)
	ADC1->SMPR1 |= ADC_SMPR1_SMP10_1; 							// sampling time (время выборки)
	ADC1->SQR1 =0; 																 // 1 regular channel (1 регулярный канал)
	ADC1->SQR3 |= ADC_SQR3_SQ1_1 | ADC_SQR3_SQ1_3; // 1е преобразование - канал 10

    for (i = 0; i < SAMP_SIZ; i++){ 						//fill array with zeros
      reads[i] = 0;
		}
    sum = 0;
    ptr = 0;

}

// filtering data by arithmetic mean
uint16_t Average(uint16_t data){

	   reader = data;  									// чтение сенсора
     sum -= reads[ptr];               // вычитание самого старого значения от суммы
		 sum += reader;                   // складвание нового значения с суммой
     reads[ptr] = reader;             // сохранение нового значения в массиве
     last = sum / SAMP_SIZ;   				// вычисление среднего
     ptr++;
     ptr %= SAMP_SIZ;

	return last;
}

uint16_t ADC_Read(void){

	//uint16_t res; // переменная для результата

	ADC1->CR2 |= ADC_CR2_SWSTART; // start conversion (запуск преобразования)
	while(!(ADC1->SR & ADC_SR_EOC)) ; // waiting for the conversion to complete (ожидание завершения преобразования)
	return ADC1->DR;

}
