#ifndef __MAX7219_H__
#define __MAX7219_H__

#include "stm32f10x.h"

typedef enum {
	REG_NO_OP 		= 0x00 << 8,
	REG_DIGIT_0 		= 0x01 << 8,
	REG_DIGIT_1 		= 0x02 << 8,
	REG_DIGIT_2 		= 0x03 << 8,
	REG_DIGIT_3 		= 0x04 << 8,
	REG_DIGIT_4 		= 0x05 << 8,
	REG_DIGIT_5 		= 0x06 << 8,
	REG_DIGIT_6 		= 0x07 << 8,
	REG_DIGIT_7 		= 0x08 << 8,
	REG_DECODE_MODE 	= 0x09 << 8,
	REG_INTENSITY 		= 0x0A << 8,
	REG_SCAN_LIMIT 		= 0x0B << 8,
	REG_SHUTDOWN 		= 0x0C << 8,
	REG_DISPLAY_TEST 	= 0x0F << 8,
} MAX7219_REGISTERS;

typedef enum {
	DIGIT_1 = 1, DIGIT_2 = 2, DIGIT_3 = 2, DIGIT_4 = 2
} MAX7219_Digits;

void max7219_init(uint8_t intensivity);

void displayTemperature(uint8_t* array);
void displayTime(uint8_t* array);
void displayEncoder(uint8_t* array);

void max7219_setIntensivity(uint8_t intensivity);

#endif /* __MAX7219_H__ */
