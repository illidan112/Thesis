#include "stm32f4xx.h"
#include "USART2.h"
#include "ADC.h"
#include "DS18B20_Driver.h"

int main(void){
	
	float T=0;
	char temp[16];
	char puls[16];
	
	ADC_Init();	
	USART2_Init();
	ds18b20_init(DS18B20_Resolution_12_bit);
	
	while(1){
		T = ds18b20_get_temperature();
		// |temp| [\][n] 
		
		sprintf(temp,"%f",T);
	//	sprintf(puls,"%d",Average(ADC_Read()));
		USART2_Send_String (temp);
	//  USART2_Send_String (" | ");
		//USART2_Send_String (puls);
		USART2_Send_String ("\r\n");
		
	}
}
