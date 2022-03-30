//--------------------------------------------------------------
// File     : USART2.h
//--------------------------------------------------------------

//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "stm32f4xx.h"

//--------------------------------------------------------------
// Global functions
//--------------------------------------------------------------
void USART2_Init(void);
void USART2_Send (char chr);
void USART2_Send_String (char* str);