/********************************** (C) COPYRIGHT *******************************
 * File Name          : usbd_composite_km.h
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2022/08/18
 * Description        : USB keyboard and mouse processing.
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for 
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/


#ifndef __USBD_COMPOSITE_KM_H
#define __USBD_COMPOSITE_KM_H

/*******************************************************************************/
/* Header Files */
#include "debug.h"
#include "string.h"
#include "usbd_desc.h"

// ����ɨ��궨��
#define ESC           0x29
#define F1            0x3A
#define F2            0x3B
#define F3            0x3C
#define F4            0x3D
#define F5            0x3E
#define F6            0x3F
#define F7            0x40
#define F8            0x41
#define F9            0x42
#define F10           0x43
#define F11           0x44
#define F12           0x45
#define PRTSC         0x46
#define DELETE        0x4C
#define BACKTICK      0x35 // "~"
#define KEY_1         0x1E
#define KEY_2         0x1F
#define KEY_3         0x20
#define KEY_4         0x21
#define KEY_5         0x22
#define KEY_6         0x23
#define KEY_7         0x24
#define KEY_8         0x25
#define KEY_9         0x26
#define KEY_0         0x27
#define MINUS         0x2D // "-"
#define EQUALS        0x2E // "+"
#define BACKSPACE     0x2A
#define TAB           0x2B
#define Q             0x14
#define W             0x1A
#define E             0x08
#define R             0x15
#define T             0x17
#define Y             0x1C
#define U             0x18
#define I             0x0C
#define O             0x12
#define P             0x13
#define LEFT_BRACKET  0x2F   // "["
#define RIGHT_BRACKET 0x30   // "]"
#define BACKSLASH     0x31   // "\"
#define CAPS_LOCK     0x39
#define A             0x04
#define S             0x16
#define D             0x07
#define F             0x09
#define G             0x0A
#define H             0x0B
#define J             0x0D
#define K             0x0E
#define L             0x0F
#define SEMICOLON     0x33   // ";"
#define QUOTE         0x34   // "'"
#define ENTER         0x28
#define LEFT_SHIFT    0x02
#define Z             0x1D
#define X             0x1B
#define C             0x06
#define V             0x19
#define B             0x05
#define N             0x11
#define M             0x10
#define COMMA         0x36   // ","
#define PERIOD        0x37   // "."
#define SLASH         0x38   // "/"
#define RIGHT_SHIFT   0x20
#define UP_ARROW      0x52   // "��"
#define LEFT_CTRL     0x01   //???
#define WIN           0x08
#define LEFT_ALT      0x04
#define SPACE         0x2C
#define RIGHT_ALT     0x40
#define FN            0xFF
#define RIGHT_CTRL    0x10
#define LEFT_ARROW    0x50  //"��"
#define DOWN_ARROW    0x51  //"��"
#define RIGHT_ARROW   0x4F  //"��"
#define NO_KEY        0x00

// ���̾��󲼾�
#define ROW_NUM 6
#define COL_NUM 18

extern uint8_t key_matrix[ROW_NUM][COL_NUM];  // ���̾���
extern uint8_t key_state[ROW_NUM * COL_NUM];  // ����״̬����



// �����Ŷ��壨����ʵ��Ӳ��������
extern uint16_t row_pins[ROW_NUM];  // ���ڿ���ÿһ�е�����
// �����Ŷ��壨PE7 �� PE15, PD8 �� PD15, PD6��
extern uint16_t col_pins[COL_NUM];  // ���ڿ���ÿһ�е�����
void KB_Scan(void);
//void KB_Scan_Handle(void);

void Keyboard_IO_Init(void);
/*******************************************************************************/
/* Global Variable Declaration */
extern volatile uint8_t  KB_LED_Last_Status;
extern volatile uint8_t  KB_LED_Cur_Status;

/*******************************************************************************/
/* Function Declaration */
extern void TIM3_Init( uint16_t arr, uint16_t psc );


extern void KB_Sleep_Wakeup_Cfg( void );

extern void KB_LED_Handle( void );



extern void USB_Sleep_Wakeup_CFG( void );
extern void MCU_Sleep_Wakeup_Operate( void );



#endif
