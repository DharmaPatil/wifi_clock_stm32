#include "main.h"

#define BUFFER_SIZE 1024
static volatile uint8_t buffer[BUFFER_SIZE];
static volatile uint8_t index = 0;
static volatile uint8_t currentData = 0;

void cleanBuffer(void) {
	for (uint32_t i = 0; i < BUFFER_SIZE; i++)
		buffer[i] = 0;
	index = 0;
}

void sound_init() {
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef SOUND;

	SOUND.GPIO_Pin = GPIO_Pin_2;
	SOUND.GPIO_Speed = GPIO_Speed_2MHz;
	SOUND.GPIO_Mode = GPIO_Mode_Out_PP;

	GPIO_Init(GPIOA, &SOUND);
}

void sound(FunctionalState state) {
	if (state == ENABLE) {
		GPIO_SetBits(GPIOA, GPIO_Pin_2);
	} else {
		GPIO_ResetBits(GPIOA, GPIO_Pin_2);
	}
}

void USART1_IRQHandler() {
	// protect
	if (index > BUFFER_SIZE) {
		index = 0;
	}
	if (USART_GetITStatus(USART1, USART_IT_RXNE)) {
		// buffer = [flag][last element][... elements ...]
		currentData = USART_ReceiveData(USART1);

		if (currentData == ':') {
			buffer[1] = 1;
		} else if (buffer[1] && (currentData != '\r') && (!buffer[0])) {
			buffer[1]++;
			buffer[buffer[1]] = currentData;
		} else if (buffer[1] && (currentData == '\r')) {
			buffer[0] = 1;
		}
	}
	USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	USART_ClearFlag(USART1, USART_IT_RXNE);
}

int main(void) {
	init_mcu();

	max7219_setIntensivity(0x08);

	uint8_t temperature[3];
	uint8_t time[4];
	uint8_t arr[4];
	uint8_t intensivity;

	setTime(12*60*60 + 0*60 + 20);

	while (1) {

		if (buffer[0] == 1) {
			switch (buffer[2]) {
			case 'P':
			default:
				break;
			case 'L':
				if (buffer[4] == 1) {
					switchLight(ENABLE);
				} else {
					switchLight(DISABLE);
				}
				break;
			case 'C':
				setTime(
						getTime(getInteger(buffer[3], buffer[4]),
								getInteger(buffer[5], buffer[6]),
								getInteger(buffer[7], buffer[8])));
				break;
			case 'I':
				max7219_setIntensivity(buffer[3]);
			}
			cleanBuffer();
		}

//		if (buffer[0] == 1) {
//			if (buffer[2] == 'P') {
//				switchLight(ENABLE);
//			} else {
//				switchLight(DISABLE);
//			}
//			cleanBuffer();
//		}

		delay(1000000);
		delay(10000);
		uint32_t tmp = RTC_GetCounter();
		uint32_t THH = 0, TMM = 0; //, TSS = 0;

		switch (getState()) {
		case DISPLAY_TIME:

			THH = (tmp / 3600) % 24;
			TMM = (tmp % 3600) / 60;
			//TSS = (tmp % 3600) % 60;
			timeToDigits(THH, TMM, time);
			displayTime(time);
			TIM3->CCR4 = 50;
			break;

		case DISPLAY_TEMPERATURE:
			temperatureToDigits(ds18b20_getTemperature(), temperature);
			displayTemperature(temperature);
			TIM3->CCR4 = 200;
			break;

		case SET_HOURS:
			cntToDigits(TIM2->CNT, arr);
			displayEncoder(arr);
			intensivityToDigits(TIM2->CNT, intensivity);
			max7219_setIntensivity(intensivity);

			TIM3->CCR4 = 400;
			break;

		case SET_MINUTES:

			break;
		default:
			pwmBlink();
			break;
		}
	}
}

void init_mcu() {
	PLLInit();

	ds18b20_init(DS18B20_Resolution_11_bit);
	max7219_init(0);
	esp8266_init();
	rtc_init();
	init_button();
	init_encoder();
	init_pwm();

	init_leds();

	sound_init();
}

