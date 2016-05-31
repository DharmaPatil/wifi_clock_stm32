#include "esp8266.h"

#define GPIOx 			GPIOA
#define UART_RX_PIN 	GPIO_Pin_10
#define UART_TX_PIN 	GPIO_Pin_9
#define UARTx 			USART1

extern void delay(uint32_t time);

extern void cleanBuffer(void);

void espInitTransmit() {
	// transmission
	transmit((uint8_t) '\n');
	transmit((uint8_t) '\r');

	char cwmode[] = "AT+CWMODE=2\n\r";
	for (int i = 0; i < 13; i++)
		transmit((uint8_t) cwmode[i]);
	delay(1000000);

	char rst[] = "AT+RST\n\r";
	for (int i = 0; i < 8; i++)
		transmit((uint8_t) rst[i]);
	for (int i = 0; i < 5; i++)
		delay(1000000);

	char cwsap[] = "AT+CWSAP=\"GATEWELL\",\"12345678\",5,0\n\r";
	for (int i = 0; i < 32; i++)
		transmit((uint8_t) cwsap[i]);
	delay(1000000);

	char cipmode[] = "AT+CIPMODE=0\n\r";
	for (int i = 0; i < 14; i++)
		transmit((uint8_t) cipmode[i]);
	delay(1000000);

	char cipmix[] = "AT+CIPMUX=1\n\r";
	for (int i = 0; i < 13; i++)
		transmit((uint8_t) cipmix[i]);
	delay(1000000);

	char server[] = "AT+CIPSERVER=1,8888\n\r";
	for (int i = 0; i < 21; i++)
		transmit((uint8_t) server[i]);
	//for (int i = 0; i < 5; i++)
	delay(1000000);
}

void esp8266_init() {
	// init gpio
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	GPIO_InitTypeDef espPins;

	/* Configure USART Tx as alternate function push-pull */
	espPins.GPIO_Mode = GPIO_Mode_AF_PP;
	espPins.GPIO_Pin = UART_TX_PIN;
	espPins.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOx, &espPins);

	/* Configure USART Rx as input floating */
	espPins.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	espPins.GPIO_Pin = UART_RX_PIN;
	GPIO_Init(GPIOx, &espPins);

	// init usart
	USART_InitTypeDef esp8266;
	esp8266.USART_BaudRate = 115200;
	esp8266.USART_WordLength = USART_WordLength_8b;
	esp8266.USART_StopBits = USART_StopBits_1;
	esp8266.USART_Parity = USART_Parity_No;
	esp8266.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	esp8266.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(UARTx, &esp8266);
	USART_Cmd(UARTx, ENABLE);
	USART_ITConfig(UARTx, USART_IT_RXNE, ENABLE);

	NVIC_InitTypeDef uartNvic;
	uartNvic.NVIC_IRQChannel = USART1_IRQn;
	uartNvic.NVIC_IRQChannelPreemptionPriority = 0;
	uartNvic.NVIC_IRQChannelSubPriority = 0;
	uartNvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&uartNvic);

	espInitTransmit();
	cleanBuffer();
}

void transmit(uint8_t data) {
	USART_SendData(UARTx, data);
	while (USART_GetFlagStatus(UARTx, USART_FLAG_TC) == RESET) {
	}
}

void transmitArray(char *c) {
	uint8_t i = 0;
	while (c[i] && (i < 255)) {
		transmit((uint8_t) c[i++]);
	}
}
