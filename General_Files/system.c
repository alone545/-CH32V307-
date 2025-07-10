#include "system.h"
#include "debug.h"
#include "../General_Files/drivers/WS2812_PWM_drv.h"
#include <math.h>

#define NUM_LEDS 6
#define PINK_COLOR 0x330033   //��ɫ

//unsigned long color1[NUM_LEDS];
//unsigned long color2[NUM_LEDS];
//unsigned long color3[NUM_LEDS];
//unsigned long color4[NUM_LEDS];
// ʹ��ѭ�������ɫ����
void fill_color(unsigned long *color_array)
{
    for (int i = 0; i < NUM_LEDS; i++)
    {
        color_array[i] = PINK_COLOR;  // ����Ϊ 20% ���ȵķ�ɫ
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
void System_Init()//��ʼ��
{
    fill_color(color1);//�������
    WS2812_Init();
}
void System_1()
{
    WS2812_SendData(color1);
}





#define NUM_LEDS 6  // �ܹ�6������
#define MAX_BRIGHTNESS 0x330033  // �������Ϊ20%��ɫ
#define MIN_BRIGHTNESS 0x110011  // ��С����Ϊ5%��ɫ�����Ը������������

unsigned long color[NUM_LEDS];  // �����洢���е������ɫ

// ��������
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
    // ��ʼ�����е���Ϊ�ر�״̬
    for (int i = 0; i < NUM_LEDS; i++) {
        color[i] = MIN_BRIGHTNESS;  // ��Ϊ��С���ȣ����ر�״̬��
    }

    // ����ָ���ĵ���
    if (led_index >= 0 && led_index < NUM_LEDS) {
        color[led_index] = MAX_BRIGHTNESS;  // ��Ϊ������ȣ����ƣ�
    }

    // ���͸��º����ɫ���ݵ� LED
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
//         0x32FFFF, 0x64FFFF, 0x96FFFF, 0xC8FFFF, 0xCFFFFF,      //��ɫ�����һ��ǰ5��
//         0xCCFFFF, 0xCCCCCC, 0xCC3333, 0xCC4444, 0xCC5555,      //��ɫ����
//         0xFF1111,0xFF0909,0xFF0707,0xFF0505,0xFF0303,          //��ɫ����
//
//         0x32FFFF, 0x64FFFF, 0x96FFFF, 0xC8FFFF, 0xCFFFFF,      //��ɫ�����2��ǰ5��
//         0xCCFFFF, 0xCCCCCC, 0xCC3333, 0xCC4444, 0xCC5555,      //��ɫ����
//         0xFF0909,0xFF0707,0xFF0505,0xFF0303, //0xFF1111,       //��ɫ����
//
//         0x32FFFF, 0x64FFFF, 0x96FFFF, 0xC8FFFF, 0xCFFFFF,      //��ɫ�����3��ǰ5��
//         0xCCFFFF, 0xCCCCCC, 0xCC3333, 0xCC4444, 0xCC5555,      //��ɫ����
//         0xFF0909,0xFF0707,0xFF0505,0xFF0303, //0xFF1111,       //��ɫ����
//
//         0x32FFFF, 0x64FFFF, 0x96FFFF, 0xC8FFFF, //0xCFFFFF,    //��ɫ�����4��ǰ5��
//         0xCCFFFF, 0xCCCCCC, 0xCC3333, 0xCC4444, 0xCC5555,      //��ɫ����
//         0xFF0909,0xFF0707,0xFF0505,0xFF0303, //0xFF1111,       //��ɫ����
//
//         0x32FFFF, 0x64FFFF, 0x96FFFF, 0xC8FFFF, //0xCFFFFF,    //��ɫ�����5��ǰ5��
//         0xCCFFFF, 0xCCCCCC, 0xCC3333, 0xCC4444, 0xCC5555,      //��ɫ����
//         0xFF0909,0xFF0707,0xFF0505,0xFF0303, //0xFF1111,       //��ɫ����
//
//         0x32FFFF, 0x64FFFF, 0x96FFFF, //0xC8FFFF, //0xCFFFFF,  //��ɫ�����һ��ǰ5��
//         0xCCCCCC,                                              //��ɫ����
//         0xFF0909,0xFF0707,0xFF0505,0xFF0303,0XEE0101,          //��ɫ����
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
