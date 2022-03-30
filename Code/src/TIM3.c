// File     : TIM3.c
//--------------------------------------------------------------

//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "stm32f4xx.h"
#include "TIM3.h"


//--------------------------------------------------------------
// Local Functions
//--------------------------------------------------------------



//--------------------------------------------------------------
// Initialization TIM3
//--------------------------------------------------------------

void Delay_Init(void){

	RCC -> APB1ENR |= RCC_APB1ENR_TIM3EN;

//	TIM3 -> CR1 = TIM_CR1_CEN; //enable timer

//	TIM3->PSC = 29999; //prescaler
//  TIM3->ARR = 15000; //load register
//	TIM3 -> CR1 = 0; //diseable timer

}

//--------------------------------------------------------------
// Pause TIM3 without interrupt
//--------------------------------------------------------------
void delay_ms(const uint32_t time){


	TIM3 -> CR1 = TIM_CR1_CEN; //enable timer
//	TIM3->PSC = 29999; //prescaler
  TIM3->ARR = TIM3_CLK / 1000000 * time; //load register
	while(!(TIM3->SR & TIM_SR_UIF)& 1){};
		TIM3->SR &= ~ TIM_SR_UIF;
		TIM3->CNT = 0; //counter reset
		TIM3 -> CR1 = 0; //diseable timer

}
