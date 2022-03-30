// File     : USART2.c
//--------------------------------------------------------------

//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "stm32f4xx.h"
#include "USART2.h"

//--------------------------------------------------------------
// Initialization USART2
//--------------------------------------------------------------
void USART2_Init(void){
	 RCC->APB1ENR |= RCC_APB1ENR_USART2EN; //Подаем тактирование на usart — 45Мгц
	 RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	//--------------Tx------------------------------

	GPIOA->MODER |= GPIO_MODER_MODER2_1; // Alternate function mode			(Режим альтернативной функции)
	GPIOA->OTYPER &= ~GPIO_OTYPER_OT_2;  // push-pull mode 					(Двухтактный выход или push-pull сокращено PP)
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR2;  // RESET 							(Сброс в 0)
	GPIOA->PUPDR |= GPIO_PUPDR_PUPDR2_0; // PULL-UP 						(Подтяжка к плюсу питания или pull-up сокращено PU)
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR2; // Setting the port speed 	(Установка скорости порта)

	//--------------Rx------------------------------
	GPIOA->MODER |= GPIO_MODER_MODER3_1; // Alternate function mode
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR3;  // RESET
	GPIOA->AFR[0] = (0x77<<(4*2));       // Assign a specific alternate function for both pins 	(Назначаем ОБОИМ выводам конкретную альтернативную функцию)

	USART2->BRR = 0x1250; // speed assignment for USART2 											(назначение скорости для USART2)
	USART2->CR1|=USART_CR1_UE|USART_CR1_TE|USART_CR1_RE; // Enable UART|reception|transmission		(Вкл. uart, приема и передачи)
	USART2->CR1|=USART_CR1_RXNEIE; //Allow to generate a receive interrupt 							(Разрешаем генерировать прерывание по приему)
	NVIC_EnableIRQ(USART2_IRQn);   //Turn on the interrupt, specify the vector						(Включаем прерывание, указываем вектор)
}
//--------------------------------------------------------------
// USART transmission of one character																(Передача по USART символа)
//--------------------------------------------------------------
void USART2_Send (char chr){
	while (!(USART2->SR & USART_SR_TC)){};
               USART2->DR = chr;
}
//--------------------------------------------------------------
// USART transmission of string
//--------------------------------------------------------------
void USART2_Send_String (char* str){
	     uint8_t i = 0;
	GPIOA->BSRRL |= GPIO_BSRR_BS_5;
       while (str[i])
			 {USART2_Send (str[i++]);}
			GPIOA->BSRRH |= GPIO_BSRR_BS_5;
		 }
