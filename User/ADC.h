/*
 * ADC.h
 *
 *  Created on: 2025��7��6��
 *      Author: Lenovo
 */

#ifndef USER_ADC_H_
#define USER_ADC_H_
#include "ch32v30x.h"

// ����ο���ѹ�� ADC �ֱ���
#define ADC_VREF 3.3f      // �ο���ѹ 3.3V
#define ADC_RESOLUTION 4095 // 12λ ADC�����ֵ 4095

// ��������

/**
 * @brief  ��ʼ�� ADC1 (PA1) ���ڵ�ص�ѹ���
 * @param  ��
 * @retval ��
 */
void ADC1_Init(void);

/**
 * @brief  ��ȡ ADC1 ��ת��ֵ
 * @param  ��
 * @retval ADC1 ת��ֵ
 */
uint16_t ADC1_GetValue(void);

/**
 * @brief  ����ѹֵת��Ϊ���ʣ������ٷֱ�
 * @param  voltage ��ص�ѹ
 * @retval ��ص����ٷֱ�
 */
float voltage_to_percentage(float voltage);

void Battery_Monitor(void);
#endif /* USER_ADC_H_ */
