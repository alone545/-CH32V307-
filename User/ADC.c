/*
 * ADC.c
 *
 *  Created on: 2025��7��6��
 *      Author: Lenovo
 */
#include "ch32v30x_gpio.h"
#include "ch32v30x.h"
#define ADC_VREF 3.3f  // ����ο���ѹΪ3.3V��ʵ�ʿ��Ը���Ӳ��������
#define ADC_RESOLUTION 4095  // 12-bit ADC�����ֵ�� 4095

// ����ӳ�䣺4.2V -> 100%��3.0V -> 0%
float voltage_to_percentage(float voltage) {
    if (voltage >= 4.2) return 100.0f; // ��ѹ����4.2VʱΪ100%
    if (voltage <= 3.0) return 0.0f;  // ��ѹС��3.0VʱΪ0%
    // ����ӳ�䣺�ѵ�ѹ�� [3.0V, 4.2V] ӳ�䵽 [0, 100]
    return (voltage - 3.0f) / (4.2f - 3.0f) * 100.0f;
}

// ADC��ʼ����PA1 -> ADC1
void ADC1_Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    ADC_InitTypeDef ADC_InitStructure;

    // ʹ��ADC1��PA1��ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    // ����PA1Ϊģ������ģʽ
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // ����ADC1
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_Init(ADC1, &ADC_InitStructure);

    // ʹ��ADC1
    ADC_Cmd(ADC1, ENABLE);
    // ʹ��ADC1��У׼
    ADC_ResetCalibration(ADC1);
    while (ADC_GetResetCalibrationStatus(ADC1));
    ADC_StartCalibration(ADC1);
    while (ADC_GetCalibrationStatus(ADC1));
}

// ��ȡADCֵ
uint16_t ADC1_GetValue(void) {
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);  // ����ADCת��
    while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);  // �ȴ�ת�����
    return ADC_GetConversionValue(ADC1);  // ��ȡת�����
}

void Battery_Monitor(void) //�ǵõ���
{
    // ��ȡADCֵ
    uint16_t adc_value = ADC1_GetValue();

    // ��ADCֵת��Ϊ��ص�ѹ������ο���ѹΪ3.3V��
    float voltage = (float)adc_value / ADC_RESOLUTION * ADC_VREF;

    // �����ص����ٷֱ�
    float battery_percentage = voltage_to_percentage(voltage);

    // ��ӡ��ص�ѹ�͵����ٷֱ�
    printf("Battery Voltage: %.2fV, Remaining Charge: %.2f%%\n", voltage, battery_percentage);

}
