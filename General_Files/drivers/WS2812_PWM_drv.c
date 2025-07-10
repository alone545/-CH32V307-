#include "WS2812_PWM_drv.h"
#include "debug.h"

#define TIMING_1  61    //(0.85/1.25)*90
#define TIMING_0  29    //(0.4/1.25)*90

void WS2812_Init()
{
/*========================================【初始化】=======================================*/
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure = {0};
    TIM_OCInitTypeDef TIM_OCInitStructure = {0};
    DMA_InitTypeDef DMA_InitStructure = {0};

    RCC_AHBPeriphClockCmd( RCC_AHBPeriph_DMA2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_TIM10 | RCC_APB2Periph_AFIO, ENABLE);

    GPIO_PinRemapConfig(GPIO_PartialRemap_TIM10, ENABLE);                  //TIM10部分映射
    GPIO_InitStructure.GPIO_Pin = WS2812_PIN;                              //TIM10 CH3/PB5
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                     //设置为复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(WS2812_PORT, &GPIO_InitStructure);

    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;    //定时器向上计数模式
    TIM_TimeBaseInitStructure.TIM_Period = 90 - 1;                      //72/0.8-1 = 89
    TIM_TimeBaseInitStructure.TIM_Prescaler = 0;
    TIM_TimeBaseInit(TIM10, &TIM_TimeBaseInitStructure);                 //初始化TIM10

    /*********************************************************************************
    TIM_OCMode_PWM1：TIM_CounterMode_Up时，当定时器CNT<CCR值时通道1为有效电平，否则为无效电平。
    TIM_OCMode_PWM2：TIM_CounterMode_Up时，当定时器CNT>CCR值时通道1为有效电平，否则为无效电平。
    *********************************************************************************/
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;                   //输出比较模式1
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;      //使能通道输出
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;           //输出比较P通道有效电平为High
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OC3Init( TIM10, &TIM_OCInitStructure);                         //初始化输出比较通道3

    DMA_DeInit(DMA2_Channel11);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&TIM10->CH3CVR;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA2_Channel11, &DMA_InitStructure );

    TIM_DMACmd(TIM10, TIM_DMA_Update, ENABLE);
    TIM_CtrlPWMOutputs(TIM10, ENABLE);
}

void WS2812_Reset()
{
/*=====================================【产生复位信号】====================================*/
    Delay_Us(300);
}

void WS2812_SendData(unsigned long *_data)
{
/*========================================【发送数据】=======================================
 * 参数：
 * _data：包含颜色信息的数组
 */

    uint16_t i, j = 0, k, buffersize;
    buffersize = WS2812_NUM * 24 + 8;
    uint16_t WS2812_buffer[buffersize];

    WS2812_Reset();
    for(k = 0;k < WS2812_NUM;k++)
    {
        for(i=8; i<16; i++)
        {
            WS2812_buffer[j] = ((_data[k]<<i) & 0x800000) ? TIMING_1:TIMING_0;//R
            j++;
        }
        for(i=0; i<8; i++)
        {
            WS2812_buffer[j] = ((_data[k]<<i) & 0x800000) ? TIMING_1:TIMING_0;//G
            j++;
        }
        for(i=16; i<24; i++)
        {
            WS2812_buffer[j] = ((_data[k]<<i) & 0x800000) ? TIMING_1:TIMING_0;//B
            j++;
        }
    }

    //莫名其妙的bug,最后一个周期波形不知道为啥全是高电平,所以增加一个周期
    for(i=0; i<8; i++)
    {
        WS2812_buffer[j] = 0;
        j++;
    }

    DMA_Cmd(DMA2_Channel11, DISABLE);
    DMA2_Channel11->MADDR = (u32)WS2812_buffer;
    DMA_SetCurrDataCounter(DMA2_Channel11, buffersize);
    DMA_Cmd(DMA2_Channel11, ENABLE);
    TIM_Cmd(TIM10, ENABLE);

    while(!DMA_GetFlagStatus(DMA2_FLAG_TC11));

    TIM_Cmd(TIM10, DISABLE);
    DMA_Cmd(DMA2_Channel11, DISABLE);
    DMA_ClearFlag(DMA2_FLAG_TC11);
}
