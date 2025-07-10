/*
 * LCDword.c
 *
 *  Created on: 2025年7月6日
 *      Author: Lenovo
 */
#include "stdio.h"
#include "LCDword.h"
#include "st7789v2.h"
#include "lcdfont.h"
#define u8 u_int8_t
#define u16 u_int16_t
/******************************************************************************
      函数说明：显示单个32x32汉字
      入口数据：x,y显示坐标
                *s 要显示的汉字
                fc 字的颜色
                bc 字的背景色
                sizey 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LCD_ShowChinese32x32(u_int16_t x,u_int16_t y,u_int8_t *s,u_int16_t fc,u_int16_t bc,u_int8_t sizey,u_int8_t mode)
{
    u8 i,j,m=0;
    u_int16_t k;
    u_int16_t HZnum;//汉字数目
    u_int16_t TypefaceNum;//一个字符所占字节大小
    //u_int16_t x0=x;
    TypefaceNum=(sizey/8+((sizey%8)?1:0))*sizey;
    HZnum=sizeof(tfont32)/sizeof(typFNT_GB32);  //统计汉字数目
    for(k=0;k<HZnum;k++)
    {
        if ((tfont32[k].Index[0]==*(s))&&(tfont32[k].Index[1]==*(s+1)))
        {
            LCD_Address_Set(x,y,x+sizey-1,y+sizey-1);
            for(i=0;i<TypefaceNum;i++)
            {
                for(j=0;j<8;j++)
                {
                    if(!mode)//非叠加方式
                    {
                        if(tfont32[k].Msk[i]&(0x01<<j))LCD_WR_DATA(fc);
                        else LCD_WR_DATA(bc);
                        m++;
                        if(m%sizey==0)
                        {
                            m=0;
                            break;
                        }
                    }
//                    else//叠加方式
//                    {
//                        if(tfont32[k].Msk[i]&(0x01<<j)) LCD_DrawPoint(x,y,fc);//画一个点
//                        x++;
//                        if((x-x0)==sizey)
//                        {
//                            x=x0;
//                            y++;
//                            break;
//                        }
//                    }
                }
            }
        }
        continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
    }
}
 /*     函数说明：显示汉字串
      入口数据：x,y显示坐标
                *s 要显示的汉字串
                fc 字的颜色
                bc 字的背景色
                sizey 字号 可选 16 24 32
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LCD_ShowChinese(u_int16_t x,u_int16_t y,u_int8_t *s,u16 fc,u_int16_t bc,u_int8_t sizey,u_int8_t mode)
{
    while(*s!=0)
    {
      if(sizey==32) LCD_ShowChinese32x32(x,y,s,fc,bc,sizey,mode);
      else return;
        s+=2;
        x+=sizey;
    }
}
/******************************************************************************
      函数说明：显示单个字符
      入口数据：x,y显示坐标
                num 要显示的字符
                fc 字的颜色
                bc 字的背景色
                sizey 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LCD_ShowChar(u16 x,u16 y,u8 num,u16 fc,u16 bc,u8 sizey,u8 mode)
{
    u8 temp,sizex,t,m=0;
    u16 i,TypefaceNum;//一个字符所占字节大小
   // u16 x0=x;
    sizex=sizey/2;
    TypefaceNum=(sizex/8+((sizex%8)?1:0))*sizey;
    num=num-' ';    //得到偏移后的值
    LCD_Address_Set(x,y,x+sizex-1,y+sizey-1);  //设置光标位置
    for(i=0;i<TypefaceNum;i++)
    {

        if(sizey==32)temp=ascii_3216[num][i];       //调用16x32字体
        else return;
        for(t=0;t<8;t++)
        {
            if(!mode)//非叠加模式
            {
                if(temp&(0x01<<t))LCD_WR_DATA(fc);
                else LCD_WR_DATA(bc);
                m++;
                if(m%sizex==0)
                {
                    m=0;
                    break;
                }
            }
//            else//叠加模式
//            {
//                if(temp&(0x01<<t))LCD_DrawPoint(x,y,fc);//画一个点
//                x++;
//                if((x-x0)==sizex)
//                {
//                    x=x0;
//                    y++;
//                    break;
//                }
//            }
        }
    }
}

/******************************************************************************
      函数说明：显示字符串
      入口数据：x,y显示坐标
                *p 要显示的字符串
                fc 字的颜色
                bc 字的背景色
                sizey 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LCD_ShowString(u16 x,u16 y,const u8 *p,u16 fc,u16 bc,u8 sizey,u8 mode)
{
    while(*p!='\0')
    {
        LCD_ShowChar(x,y,*p,fc,bc,sizey,mode);
        x+=sizey/2;
        p++;
    }
}
