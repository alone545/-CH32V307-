/*
 * LCDword.c
 *
 *  Created on: 2025��7��6��
 *      Author: Lenovo
 */
#include "stdio.h"
#include "LCDword.h"
#include "st7789v2.h"
#include "lcdfont.h"
#define u8 u_int8_t
#define u16 u_int16_t
/******************************************************************************
      ����˵������ʾ����32x32����
      ������ݣ�x,y��ʾ����
                *s Ҫ��ʾ�ĺ���
                fc �ֵ���ɫ
                bc �ֵı���ɫ
                sizey �ֺ�
                mode:  0�ǵ���ģʽ  1����ģʽ
      ����ֵ��  ��
******************************************************************************/
void LCD_ShowChinese32x32(u_int16_t x,u_int16_t y,u_int8_t *s,u_int16_t fc,u_int16_t bc,u_int8_t sizey,u_int8_t mode)
{
    u8 i,j,m=0;
    u_int16_t k;
    u_int16_t HZnum;//������Ŀ
    u_int16_t TypefaceNum;//һ���ַ���ռ�ֽڴ�С
    //u_int16_t x0=x;
    TypefaceNum=(sizey/8+((sizey%8)?1:0))*sizey;
    HZnum=sizeof(tfont32)/sizeof(typFNT_GB32);  //ͳ�ƺ�����Ŀ
    for(k=0;k<HZnum;k++)
    {
        if ((tfont32[k].Index[0]==*(s))&&(tfont32[k].Index[1]==*(s+1)))
        {
            LCD_Address_Set(x,y,x+sizey-1,y+sizey-1);
            for(i=0;i<TypefaceNum;i++)
            {
                for(j=0;j<8;j++)
                {
                    if(!mode)//�ǵ��ӷ�ʽ
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
//                    else//���ӷ�ʽ
//                    {
//                        if(tfont32[k].Msk[i]&(0x01<<j)) LCD_DrawPoint(x,y,fc);//��һ����
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
        continue;  //���ҵ���Ӧ�����ֿ������˳�����ֹ��������ظ�ȡģ����Ӱ��
    }
}
 /*     ����˵������ʾ���ִ�
      ������ݣ�x,y��ʾ����
                *s Ҫ��ʾ�ĺ��ִ�
                fc �ֵ���ɫ
                bc �ֵı���ɫ
                sizey �ֺ� ��ѡ 16 24 32
                mode:  0�ǵ���ģʽ  1����ģʽ
      ����ֵ��  ��
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
      ����˵������ʾ�����ַ�
      ������ݣ�x,y��ʾ����
                num Ҫ��ʾ���ַ�
                fc �ֵ���ɫ
                bc �ֵı���ɫ
                sizey �ֺ�
                mode:  0�ǵ���ģʽ  1����ģʽ
      ����ֵ��  ��
******************************************************************************/
void LCD_ShowChar(u16 x,u16 y,u8 num,u16 fc,u16 bc,u8 sizey,u8 mode)
{
    u8 temp,sizex,t,m=0;
    u16 i,TypefaceNum;//һ���ַ���ռ�ֽڴ�С
   // u16 x0=x;
    sizex=sizey/2;
    TypefaceNum=(sizex/8+((sizex%8)?1:0))*sizey;
    num=num-' ';    //�õ�ƫ�ƺ��ֵ
    LCD_Address_Set(x,y,x+sizex-1,y+sizey-1);  //���ù��λ��
    for(i=0;i<TypefaceNum;i++)
    {

        if(sizey==32)temp=ascii_3216[num][i];       //����16x32����
        else return;
        for(t=0;t<8;t++)
        {
            if(!mode)//�ǵ���ģʽ
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
//            else//����ģʽ
//            {
//                if(temp&(0x01<<t))LCD_DrawPoint(x,y,fc);//��һ����
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
      ����˵������ʾ�ַ���
      ������ݣ�x,y��ʾ����
                *p Ҫ��ʾ���ַ���
                fc �ֵ���ɫ
                bc �ֵı���ɫ
                sizey �ֺ�
                mode:  0�ǵ���ģʽ  1����ģʽ
      ����ֵ��  ��
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
