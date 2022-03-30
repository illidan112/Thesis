// File     : DS18B20_Driver.c
//--------------------------------------------------------------

//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "stm32f4xx.h"
#include "DS18B20_Driver.h"
#include "TIM3.h"


//--------------------------------------------------------------
// Local Functions
//--------------------------------------------------------------

uint16_t read_temperature(void);
uint8_t read_bit(void);
uint8_t get_devider(const  DS18B20_RESOLUTION_t resolution);
uint8_t read_bit(void);
void reset(void);
void write_byte(uint8_t data);
void write_bit(uint8_t bit);

//--------------------------------------------------------------
// Инициализация DS18B20
//--------------------------------------------------------------
void ds18b20_init(const  DS18B20_RESOLUTION_t resolution) {
 	Delay_Init();

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	// output open-drain
	GPIOA->MODER |= GPIO_MODER_MODER12_0;
	GPIOA->OTYPER |= GPIO_OTYPER_OT_12;
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR12;


	ds18b20_set_resolution(resolution);
}

//--------------------------------------------------------------
// Сменить точность
//--------------------------------------------------------------
void ds18b20_set_resolution(const  DS18B20_RESOLUTION_t resolution) {
    reset();                      // send reset
    write_byte(SKIP_ROM);         // work only with one device
    write_byte(WRITE_SCRATCHPAD); // set resolution
    write_byte(TH_REGISTER);      //
    write_byte(TL_REGISTER);      //
    write_byte(resolution);       //
		convert_delay = DELAY_T_CONVERT / get_devider(resolution); // calc conversation delay
}

uint8_t get_devider(const  DS18B20_RESOLUTION_t resolution) {
    uint8_t devider;
    switch (resolution) {
    case DS18B20_Resolution_9_bit:
        devider = 8;
        break;
    case DS18B20_Resolution_10_bit:
        devider = 4;
        break;
    case DS18B20_Resolution_11_bit:
        devider = 2;
        break;
    case DS18B20_Resolution_12_bit:
    default:
        devider = 1;
        break;
    }

    return devider;
}
//--------------------------------------------------------------
// RESET                                                   СБРОС
//--------------------------------------------------------------
void reset(void) {
    GPIOA->ODR &= ~GPIO_ODR_ODR_12;
    delay_ms(DELAY_RESET);
    GPIOA->ODR |= GPIO_ODR_ODR_12;
    delay_ms(DELAY_RESET);
}
//--------------------------------------------------------------
// Get temperature value          Получение значения температуры
//--------------------------------------------------------------
float ds18b20_get_temperature() {
    reset();
    write_byte(SKIP_ROM);
    write_byte(CONVERT_T);

    delay_ms(convert_delay);

    reset();
    write_byte(SKIP_ROM);
    write_byte(READ_SCRATCHPAD);

    return ((float) read_temperature() / 16.0f);;
}

//--------------------------------------------------------------
// Sending a BIT
//--------------------------------------------------------------
void write_bit(uint8_t bit) {
    GPIOA->ODR &= ~GPIO_ODR_ODR_12;
    delay_ms(bit ? DELAY_WRITE_1 : DELAY_WRITE_0);
    GPIOA->ODR |= GPIO_ODR_ODR_12;
    delay_ms(bit ? DELAY_WRITE_1_PAUSE : DELAY_WRITE_0_PAUSE);
}

//--------------------------------------------------------------
// Sending a BYTE
//--------------------------------------------------------------
void write_byte(uint8_t data) {
	uint16_t i;
    for (i = 0; i < 8; i++){
        write_bit(data >> i & 1);
        delay_ms(DELAY_RELAXATION);
    }
}

//--------------------------------------------------------------
// Reading a bit
//--------------------------------------------------------------
uint8_t read_bit(void) {
    uint8_t bit = 0;
    GPIOA->ODR &= ~GPIO_ODR_ODR_12;
    delay_ms(DELAY_READ_SLOT);
    GPIOA->ODR |= GPIO_ODR_ODR_12;
    // ... switch to INPUT
    delay_ms(DELAY_BUS_RELAX);
    bit = (GPIOA->IDR & GPIO_IDR_IDR_12 ? 1 : 0);
    delay_ms(DELAY_READ_PAUSE);
    // ... switch to OUTPUT
    return bit;
}

//--------------------------------------------------------------
// Reading 2 bytes
//--------------------------------------------------------------
uint16_t read_temperature(void) {
  uint16_t data = 0;

	uint8_t i;
	    for (i = 0; i < 16; i++)
        data += (uint16_t) read_bit() << i;
	    //return (uint16_t)(((float) data / 16.0f) * 10.0f);

			//return ((float) data / 16.0f);

			//uint16_t rtrn = data * 0.0625f;
			//return (uint16_t)(((float) data * 0.0625f));
			return data;
}
