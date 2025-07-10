#ifndef __WS2812_PWM_DRV_H
#define __WS2812_PWM_DRV_H

#define WS2812_NUM      79
#define WS2812_PORT     GPIOB
#define WS2812_PIN      GPIO_Pin_5

void WS2812_Init();
void WS2812_Reset();
//void WS2812_SendData(unsigned long *_data);

#endif
