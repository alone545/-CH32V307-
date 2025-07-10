#include "system.h"
#include "debug.h"
#include "../General_Files/drivers/WS2812_PWM_drv.h"
#include <math.h>

#define NUM_LEDS 6
#define PINK_COLOR 0x330033   //粉色

//unsigned long color1[NUM_LEDS];
//unsigned long color2[NUM_LEDS];
//unsigned long color3[NUM_LEDS];
//unsigned long color4[NUM_LEDS];
// 使用循环填充颜色数组
void fill_color(unsigned long *color_array)
{
    for (int i = 0; i < NUM_LEDS; i++)
    {
        color_array[i] = PINK_COLOR;  // 设置为 20% 亮度的粉色
    }
}

unsigned long color1[WS2812_NUM]=
{
        0x330033,
        0x00ff00,
        0x0000ff,
        0xff3300,
        0xff00ff,
        0x0033ff,

};
unsigned long color2[WS2812_NUM]=
{
        0x00ff00,
        0x0000ff,
        0xff0000,
        0x00ffff,
        0xff00ff,
        0xffff00,
};
unsigned long color3[WS2812_NUM]=
{
        0x0000ff,
        0xff0000,
        0x00ff00,
        0xffff00,
        0x00ffff,
        0xff00ff,
};
unsigned long color4[WS2812_NUM]=
{
        0xffffff,
        0xffffff,
        0xffffff,
        0xffffff,
        0xffffff,
        0xffffff,
};
unsigned long color8[WS2812_NUM]=
{
        0xff0000,
        0x00ff00,
        0x0000ff,
        0xffff00,
        0xff00ff,
        0x00ffff,
};
void System_Init()//初始化
{
    fill_color(color1);//填充数组
    WS2812_Init();
}
void System_1()
{
    WS2812_SendData(color1);
}





#define NUM_LEDS 6  // 总共6个灯珠
#define MAX_BRIGHTNESS 0x330033  // 最大亮度为20%粉色
#define MIN_BRIGHTNESS 0x110011  // 最小亮度为5%粉色（可以根据需求调整）

unsigned long color[NUM_LEDS];  // 用来存储所有灯珠的颜色

// 设置亮度
void set_brightness(unsigned long *color, float brightness_factor)
{
    unsigned long r = (color[0] * brightness_factor);
    unsigned long g = (color[1] * brightness_factor);
    unsigned long b = (color[2] * brightness_factor);

    color[0] = (r > 255) ? 255 : r;
    color[1] = (g > 255) ? 255 : g;
    color[2] = (b > 255) ? 255 : b;
}

void led(int led_index)
{
    // 初始化所有灯珠为关闭状态
    for (int i = 0; i < NUM_LEDS; i++) {
        color[i] = MIN_BRIGHTNESS;  // 设为最小亮度（即关闭状态）
    }

    // 点亮指定的灯珠
    if (led_index >= 0 && led_index < NUM_LEDS) {
        color[led_index] = MAX_BRIGHTNESS;  // 设为最大亮度（亮灯）
    }

    // 发送更新后的颜色数据到 LED
    WS2812_SendData(color);
}

unsigned long color5[WS2812_NUM]=
{
0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,
0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,
0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,
0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,
0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,
0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,0xFFFFFF,
//         0x32FFFF, 0x64FFFF, 0x96FFFF, 0xC8FFFF, 0xCFFFFF,      //蓝色渐变第一行前5列
//         0xCCFFFF, 0xCCCCCC, 0xCC3333, 0xCC4444, 0xCC5555,      //白色渐变
//         0xFF1111,0xFF0909,0xFF0707,0xFF0505,0xFF0303,          //红色渐变
//
//         0x32FFFF, 0x64FFFF, 0x96FFFF, 0xC8FFFF, 0xCFFFFF,      //蓝色渐变第2行前5列
//         0xCCFFFF, 0xCCCCCC, 0xCC3333, 0xCC4444, 0xCC5555,      //白色渐变
//         0xFF0909,0xFF0707,0xFF0505,0xFF0303, //0xFF1111,       //红色渐变
//
//         0x32FFFF, 0x64FFFF, 0x96FFFF, 0xC8FFFF, 0xCFFFFF,      //蓝色渐变第3行前5列
//         0xCCFFFF, 0xCCCCCC, 0xCC3333, 0xCC4444, 0xCC5555,      //白色渐变
//         0xFF0909,0xFF0707,0xFF0505,0xFF0303, //0xFF1111,       //红色渐变
//
//         0x32FFFF, 0x64FFFF, 0x96FFFF, 0xC8FFFF, //0xCFFFFF,    //蓝色渐变第4行前5列
//         0xCCFFFF, 0xCCCCCC, 0xCC3333, 0xCC4444, 0xCC5555,      //白色渐变
//         0xFF0909,0xFF0707,0xFF0505,0xFF0303, //0xFF1111,       //红色渐变
//
//         0x32FFFF, 0x64FFFF, 0x96FFFF, 0xC8FFFF, //0xCFFFFF,    //蓝色渐变第5行前5列
//         0xCCFFFF, 0xCCCCCC, 0xCC3333, 0xCC4444, 0xCC5555,      //白色渐变
//         0xFF0909,0xFF0707,0xFF0505,0xFF0303, //0xFF1111,       //红色渐变
//
//         0x32FFFF, 0x64FFFF, 0x96FFFF, //0xC8FFFF, //0xCFFFFF,  //蓝色渐变第一行前5列
//         0xCCCCCC,                                              //白色渐变
//         0xFF0909,0xFF0707,0xFF0505,0xFF0303,0XEE0101,          //红色渐变
};

void System_2()
{
    WS2812_SendData(color8);
}
void System_Loop()
{
    WS2812_SendData(color1);
    Delay_Ms(200);
    WS2812_SendData(color2);
    Delay_Ms(200);
    WS2812_SendData(color3);
    Delay_Ms(200);
//    WS2812_SendData(color4);
//    Delay_Ms(200);
}
