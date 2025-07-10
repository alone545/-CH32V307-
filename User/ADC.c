/*
 * ADC.c
 *
 *  Created on: 2025年7月6日
 *      Author: Lenovo
 */
#include "ch32v30x_gpio.h"
#include "ch32v30x.h"
#define ADC_VREF 3.3f  // 假设参考电压为3.3V（实际可以根据硬件调整）
#define ADC_RESOLUTION 4095  // 12-bit ADC，最大值是 4095

// 线性映射：4.2V -> 100%，3.0V -> 0%
float voltage_to_percentage(float voltage) {
    if (voltage >= 4.2) return 100.0f; // 电压大于4.2V时为100%
    if (voltage <= 3.0) return 0.0f;  // 电压小于3.0V时为0%
    // 线性映射：把电压从 [3.0V, 4.2V] 映射到 [0, 100]
    return (voltage - 3.0f) / (4.2f - 3.0f) * 100.0f;
}

// ADC初始化：PA1 -> ADC1
void ADC1_Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    ADC_InitTypeDef ADC_InitStructure;

    // 使能ADC1和PA1的时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    // 配置PA1为模拟输入模式
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 配置ADC1
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_Init(ADC1, &ADC_InitStructure);

    // 使能ADC1
    ADC_Cmd(ADC1, ENABLE);
    // 使能ADC1的校准
    ADC_ResetCalibration(ADC1);
    while (ADC_GetResetCalibrationStatus(ADC1));
    ADC_StartCalibration(ADC1);
    while (ADC_GetCalibrationStatus(ADC1));
}

// 获取ADC值
uint16_t ADC1_GetValue(void) {
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);  // 启动ADC转换
    while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);  // 等待转换完成
    return ADC_GetConversionValue(ADC1);  // 获取转换结果
}

void Battery_Monitor(void) //记得调用
{
    // 获取ADC值
    uint16_t adc_value = ADC1_GetValue();

    // 将ADC值转换为电池电压（假设参考电压为3.3V）
    float voltage = (float)adc_value / ADC_RESOLUTION * ADC_VREF;

    // 计算电池电量百分比
    float battery_percentage = voltage_to_percentage(voltage);

    // 打印电池电压和电量百分比
    printf("Battery Voltage: %.2fV, Remaining Charge: %.2f%%\n", voltage, battery_percentage);

}
