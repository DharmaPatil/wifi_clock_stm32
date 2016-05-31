#ifndef __DS18B20_H__
#define __DS18B20_H__

#include "stm32f10x.h"

/**
 * ������������ ���� ��������� ������������.
 *   ���.  �����
 *  /   \/       \
 * 0 X Y 1 1 1 1 1
 *  ________________
 * |__���.__|_X_|_Y_|
 * |    9	| 0 | 0 |
 * |   10	| 0 | 1 |
 * |   11	| 1 | 0 |
 * |___12___|_1_|_1_|
 */
typedef enum {
	DS18B20_Resolution_9_bit 	= 0x1F,
	DS18B20_Resolution_10_bit 	= 0x3F,
	DS18B20_Resolution_11_bit 	= 0x5F,
	DS18B20_Resolution_12_bit 	= 0x7F
} DS18B20_Resolution;

/**
 * �������� ������������ ������������ ����� �
 * ����������� ����������� �����.
 */
void ds18b20_init(DS18B20_Resolution resolution);

void setResolution(DS18B20_Resolution resolution);

/**
 * @param ����������� ���
 * @return ����������� � ������������� �����, ������� ��� -- �������
 */
uint16_t ds18b20_getTemperature();

#endif /* __DS18B20_H__ */
