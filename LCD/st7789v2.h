/*
 * st7789v2.h
 *
 *  Created on: 2025年7月6日
 *      Author: Lenovo
 */

#ifndef LCD_ST7789V2_H_
#define LCD_ST7789V2_H_

#include "stdio.h"
#include "config.h"

//   FLASH_CS   -- PA2
//    FLASH_DO   -- PA6(SPI1_MISO)
//    FLASH_DI   -- PA7(SPI1_MOSI)
//    FLASH_SLK  -- PA5(SPI1_SCK)
//
//    LCD_BLK    -- PB9
//    LCD_DC     -- PB10
//    LCD_RES    -- PB11
//    LCD_CS1    -- PB12
//    LCD_SCL    -- PB13(SPI2_SCK)
//    LCD_SDA    -- PB15(SPI2_MOSI)

#define LCD_SCLK_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_13)//SCL=SCLK
#define LCD_SCLK_Set() GPIO_SetBits(GPIOB,GPIO_Pin_13)

#define LCD_MOSI_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_15)//SDA=MOSI
#define LCD_MOSI_Set() GPIO_SetBits(GPIOB,GPIO_Pin_15)

#define LCD_RES_Clr()  GPIO_ResetBits(GPIOB,GPIO_Pin_11)//RES
#define LCD_RES_Set()  GPIO_SetBits(GPIOB,GPIO_Pin_11)

#define LCD_DC_Clr()   GPIO_ResetBits(GPIOB,GPIO_Pin_10)//DC
#define LCD_DC_Set()   GPIO_SetBits(GPIOB,GPIO_Pin_10)

#define LCD_CS_Clr()   GPIO_ResetBits(GPIOB,GPIO_Pin_12)//CS
#define LCD_CS_Set()   GPIO_SetBits(GPIOB,GPIO_Pin_12)

#define LCD_BLK_Clr()  GPIO_ResetBits(GPIOB,GPIO_Pin_9)//BLK
#define LCD_BLK_Set()  GPIO_SetBits(GPIOB,GPIO_Pin_9)




void LCD_GPIO_Init(void);//初始化GPIO
void LCD_Writ_Bus(u_int8_t dat);//模拟SPI时序
void LCD_WR_DATA8(u_int8_t dat);//写入一个字节
void LCD_WR_DATA(u_int16_t dat);//写入两个字节
void LCD_WR_REG(u_int8_t dat);//写入一个指令
void LCD_Address_Set(u_int16_t x1,u_int16_t y1,u_int16_t x2,u_int16_t y2);//设置坐标函数
void LCD_Init(void);//LCD初始化



#endif /* LCD_ST7789V2_H_ */
