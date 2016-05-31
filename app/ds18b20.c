#include "ds18b20.h"

extern void delay(uint32_t time);

#define GPIOx 					GPIOA
#define PIN 					GPIO_Pin_15

#define DELAY_RESET 			500
#define DELAY_WRITE_0			60
#define DELAY_WRITE_0_PAUSE 	10
#define DELAY_WRITE_1			10
#define DELAY_WRITE_1_PAUSE 	60
#define DELAY_READ_SLOT 		10
#define DELAY_BUS_RELAX			10
#define DELAY_READ_PAUSE		50
#define DELAY_T_CONVERT			760000
#define DELAT_PROTECTION		5

typedef enum {
	SKIP_ROM = 0xCC,
	CONVERT_T = 0x44,
	READ_SCRATCHPAD = 0xBE,
	WRITE_SCRATCHPAD = 0x4E,
	TH_REGISTER = 0x4B,
	TL_REGISTER = 0x46,
} COMMANDS;

uint32_t DELAY_WAIT_CONVERT = DELAY_T_CONVERT;

GPIO_InitTypeDef DS18B20;

void ds18b20_init(DS18B20_Resolution resolution) {
	// JTAG disable
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
	AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_1; // AFIO_MAPR_SWJ_CFG_2 ??
	RCC->APB2ENR &= ~RCC_APB2ENR_AFIOEN;

	DS18B20.GPIO_Pin = PIN;
	DS18B20.GPIO_Speed = GPIO_Speed_50MHz;
	DS18B20.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_Init(GPIOx, &DS18B20);

	setResolution(resolution);
}

void reset() {
	GPIOx->ODR &= ~PIN;
	delay(DELAY_RESET);
	GPIOx->ODR |= PIN;
	delay(DELAY_RESET);
}

void writeBit(uint8_t bit) {
	GPIOx->ODR &= ~PIN;
	delay(bit ? DELAY_WRITE_1 : DELAY_WRITE_0);
	GPIOx->ODR |= PIN;
	delay(bit ? DELAY_WRITE_1_PAUSE : DELAY_WRITE_0_PAUSE);
}

void writeByte(uint8_t data) {
	for (uint8_t i = 0; i < 8; i++) {
		writeBit(data >> i & 1);
		delay(DELAT_PROTECTION);
	}
}

uint8_t readBit() {
	uint8_t bit = 0;
	GPIOx->ODR &= ~PIN;
	delay(DELAY_READ_SLOT);
	GPIOx->ODR |= PIN;
	// FIXME: switch to INPUT
	delay(DELAY_BUS_RELAX);
	bit = (GPIOx->IDR & PIN ? 1 : 0);
	delay(DELAY_READ_PAUSE);
	// FIXME: switch to OUTPUT
	return bit;
}

uint16_t readTemperature() {
	uint16_t data = 0;
	for (uint8_t i = 0; i < 16; i++)
		data += (uint16_t) readBit() << i;
	return (uint16_t)(((float) data / 16.0) * 10.0);
}

uint8_t getDevider(DS18B20_Resolution resolution) {
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
	}
	return devider;
}

void setResolution(DS18B20_Resolution resolution) {
	reset();
	writeByte(SKIP_ROM);
	writeByte(WRITE_SCRATCHPAD);
	writeByte(TH_REGISTER);
	writeByte(TL_REGISTER);
	writeByte(resolution);
	DELAY_WAIT_CONVERT = DELAY_T_CONVERT / getDevider(resolution);
}

uint16_t ds18b20_getTemperature() {

	reset();
	writeByte(SKIP_ROM);
	writeByte(CONVERT_T);
	delay(DELAY_WAIT_CONVERT);

	reset();
	writeByte(SKIP_ROM);
	writeByte(READ_SCRATCHPAD);

	return readTemperature();
}

