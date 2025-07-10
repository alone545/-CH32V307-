/*
 * ADC.h
 *
 *  Created on: 2025年7月6日
 *      Author: Lenovo
 */

#ifndef USER_ADC_H_
#define USER_ADC_H_
#include "ch32v30x.h"

// 定义参考电压和 ADC 分辨率
#define ADC_VREF 3.3f      // 参考电压 3.3V
#define ADC_RESOLUTION 4095 // 12位 ADC，最大值 4095

// 函数声明

/**
 * @brief  初始化 ADC1 (PA1) 用于电池电压检测
 * @param  无
 * @retval 无
 */
void ADC1_Init(void);

/**
 * @brief  获取 ADC1 的转换值
 * @param  无
 * @retval ADC1 转换值
 */
uint16_t ADC1_GetValue(void);

/**
 * @brief  将电压值转换为电池剩余电量百分比
 * @param  voltage 电池电压
 * @retval 电池电量百分比
 */
float voltage_to_percentage(float voltage);

void Battery_Monitor(void);
#endif /* USER_ADC_H_ */
