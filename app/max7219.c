#include "max7219.h"

#define SPIx 		SPI1
#define SPI_SCK 	GPIO_Pin_5
#define SPI_CS 		GPIO_Pin_6
#define SPI_MOSI 	GPIO_Pin_7
#define SPI_GPIO 	GPIOA

void sendData(uint16_t data) {
	SPI_I2S_SendData(SPIx, data);
	// waiting for sending
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_BSY) == SET)
		;
	GPIO_ResetBits(SPI_GPIO, SPI_CS);
	//asm("nop");
	GPIO_SetBits(SPI_GPIO, SPI_CS);
}

void max7219_setIntensivity(uint8_t intensivity) {
	if (intensivity > 0x0F)
		return;
	sendData(REG_SHUTDOWN | 0x01);
	sendData(REG_DECODE_MODE | 0x00);
	sendData(REG_SCAN_LIMIT | 0x03);
	sendData(REG_INTENSITY | intensivity);
}

void max7219_clean() {
	sendData(REG_DIGIT_0 | 0x00);
	sendData(REG_DIGIT_1 | 0x00);
	sendData(REG_DIGIT_2 | 0x00);
	sendData(REG_DIGIT_3 | 0x00);
}

void max7219_init(uint8_t intensivity) {
	// gpio clock
	RCC_APB2PeriphClockCmd(RCC_APB2ENR_IOPAEN, ENABLE);
	// remap
	//GPIO_PinRemapConfig(GPIO_Remap_SPI1, ENABLE);
	// spi clock
	RCC_APB2PeriphClockCmd(RCC_APB2ENR_SPI1EN, ENABLE);

	// structures
	GPIO_InitTypeDef pins;
	SPI_InitTypeDef spi;

	// init pins
	pins.GPIO_Mode = GPIO_Mode_AF_PP;
	pins.GPIO_Speed = GPIO_Speed_50MHz;
	pins.GPIO_Pin = SPI_SCK | SPI_MOSI;
	GPIO_Init(SPI_GPIO, &pins);

	// init chip select
	pins.GPIO_Mode = GPIO_Mode_Out_PP;
	pins.GPIO_Pin = SPI_CS;
	GPIO_Init(GPIOA, &pins);
	GPIO_SetBits(SPI_GPIO, SPI_CS);

	//
	spi.SPI_Direction = SPI_Direction_1Line_Tx; // transmit only!
	spi.SPI_DataSize = SPI_DataSize_16b; // see datasheet!
	spi.SPI_CPOL = SPI_CPOL_Low;
	spi.SPI_CPHA = SPI_CPHA_1Edge;
	spi.SPI_NSS = SPI_NSS_Soft; // soft managment?
	spi.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
	spi.SPI_FirstBit = SPI_FirstBit_MSB;
	spi.SPI_Mode = SPI_Mode_Master; // master mode

	SPI_Init(SPIx, &spi); // init SPI1
	SPI_Cmd(SPIx, ENABLE); // turn on SPI1

	SPI_NSSInternalSoftwareConfig(SPIx, SPI_NSSInternalSoft_Set);

	max7219_setIntensivity(intensivity);
	max7219_clean();
}

static uint16_t SYMBOLS[] = { 0x7E, 0x30, 0x6D, 0x79, 0x33, 0x5B, 0x5F, 0x70,
		0x7F, 0x7B, 0x00 };

uint16_t getSymbol(uint8_t number) {
	return SYMBOLS[number];
}

void displayTime(uint8_t* array) {
	sendData(REG_SHUTDOWN | 0x01);
	sendData(REG_DECODE_MODE | 0x00);
	sendData(REG_SCAN_LIMIT | 0x03);
	sendData(REG_DIGIT_0 | getSymbol(array[0]));
	sendData(REG_DIGIT_1 | getSymbol(array[1]) | (1 << 7));
	sendData(REG_DIGIT_2 | getSymbol(array[2]));
	sendData(REG_DIGIT_3 | getSymbol(array[3]));
}

void displayTemperature(uint8_t* array) {
	sendData(REG_SHUTDOWN | 0x01);
	sendData(REG_DECODE_MODE | 0x00);
	sendData(REG_SCAN_LIMIT | 0x03);
	sendData(REG_DIGIT_0 | 0x00);
	sendData(REG_DIGIT_1 | getSymbol(array[0]));
	sendData(REG_DIGIT_2 | getSymbol(array[1]) | (1 << 7));
	sendData(REG_DIGIT_3 | getSymbol(array[2]));
}

void displayEncoder(uint8_t* array) {
	sendData(REG_SHUTDOWN | 0x01);
	sendData(REG_DECODE_MODE | 0x00);
	sendData(REG_SCAN_LIMIT | 0x03);
	sendData(REG_DIGIT_0 | 0x00);
	sendData(REG_DIGIT_1 | getSymbol(array[1]));
	sendData(REG_DIGIT_2 | getSymbol(array[2]));
	sendData(REG_DIGIT_3 | getSymbol(array[3]));
}

//void max7219_setValue(uint8_t* value, uint8_t dot) {
//	sendData(REG_SHUTDOWN | 0x01);
//	sendData(REG_DECODE_MODE | 0x00);
//	sendData(REG_SCAN_LIMIT | 0x03);
//}
