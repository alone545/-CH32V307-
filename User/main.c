 /* pins:
    FLASH_CS   -- PA2
    FLASH_DO   -- PA6(SPI1_MISO)
    FLASH_DI   -- PA7(SPI1_MOSI)
    FLASH_SLK  -- PA5(SPI1_SCK)

    LCD_BLK    -- PB9
    LCD_DC     -- PB10
    LCD_RES    -- PB11
    LCD_CS1    -- PB12
    LCD_SCL    -- PB13(SPI2_SCK)
    LCD_SDA    -- PB15(SPI2_MOSI)

    CH32_PA9   -- ESP P9�� RX��
    CH32_PA10  -- ESP P10��TX��

*/

/*******************************************************************************/
/* Header Files */
#include "ch32v30x_usbfs_device.h"
#include "usbd_composite_km.h"
#include "LCDword.h"
#include "st7789v2.h"
#include "debug.h"
#include "string.h"
#include "ch32v30x_it.h"
#include "flash.h"
#include "config.h"
#include "lcd.h"
#include "system.h"
#include "WS2812_PWM_drv.h"
#include  "ADC.h"
/*********************************************************************
 * @fn      main
 *
 * @brief   Main program
 *
 * @return  none
 */
//static void UART2_Init(uint32_t baud)
//{
//    USART_InitTypeDef USART_InitStructure;
//    GPIO_InitTypeDef  GPIO_InitStructure;
//
//    // ʹ�� GPIOA ʱ��
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
//    // ʹ�� USART2 ʱ�ӣ�USART2 λ�� APB1 �����ϣ�
//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
//
//    // ���� PA2 Ϊ�������� (TX)
//    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
//    GPIO_Init(GPIOA, &GPIO_InitStructure);
//
//    // ���� PA3 Ϊ�������� (RX)
//    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//    GPIO_Init(GPIOA, &GPIO_InitStructure);
//
//    // USART2 ��������
//    USART_InitStructure.USART_BaudRate            = baud;
//    USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
//    USART_InitStructure.USART_StopBits            = USART_StopBits_1;
//    USART_InitStructure.USART_Parity              = USART_Parity_No;
//    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//    USART_InitStructure.USART_Mode                = USART_Mode_Tx | USART_Mode_Rx;
//    USART_Init(USART2, &USART_InitStructure);
//
//    // ʹ�� USART2
//    USART_Cmd(USART2, ENABLE);
//}
static void UART1_Init(uint32_t baud)
{
    USART_InitTypeDef USART_InitStructure;
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate            = baud;
    USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits            = USART_StopBits_1;
    USART_InitStructure.USART_Parity              = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode                = USART_Mode_Tx | USART_Mode_Rx;
    USART_Init(USART1, &USART_InitStructure);

    USART_Cmd(USART1, ENABLE);
}
static void UART2_Init(uint32_t baud)
{
    USART_InitTypeDef USART_InitStructure;
    GPIO_InitTypeDef  GPIO_InitStructure;

    // ʹ�� GPIOA ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    // ʹ�� USART2 ʱ�ӣ�USART2 λ�� APB1 �����ϣ�
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

    // ���� PA2 Ϊ�������� (TX)
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // ���� PA3 Ϊ�������� (RX)
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // USART2 ��������
    USART_InitStructure.USART_BaudRate            = baud;
    USART_InitStructure.USART_WordLength          = USART_WordLength_8b;  // 8 ����λ
    USART_InitStructure.USART_StopBits            = USART_StopBits_1;      // 1 ֹͣλ
    USART_InitStructure.USART_Parity              = USART_Parity_No;     // ʹ��żУ��λ����Ϊ USART_Parity_Odd ʹ����У��
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode                = USART_Mode_Tx | USART_Mode_Rx;

    // ��ʼ�� USART2
    USART_Init(USART2, &USART_InitStructure);

    // ʹ�� USART2
    USART_Cmd(USART2, ENABLE);
}

// LCD ������ʾ������������ѭ����������
void LCD_DisplayAnimation(void)
{
    uint32_t addr = 134400;
    for (uint16_t i = 0; i < 9; i++) {
        LCD_drawImageWithSize(20, 0, LCD_W, LCD_H, addr);
        addr += (LCD_W * LCD_H * COLOR_BYTE);

    }
}

int main( void )
{
	SystemCoreClockUpdate( );
    NVIC_PriorityGroupConfig( NVIC_PriorityGroup_2 );
	Delay_Init( );
	UART1_Init(115200);
	KB_Sleep_Wakeup_Cfg( );
	TIM3_Init( 1, SystemCoreClock / 10000 - 1 );
	UART2_Init(115200);
	USBFS_RCC_Init( );
	USBFS_Device_Init( ENABLE );
	USB_Sleep_Wakeup_CFG( );
//	GPIO_SetBits(GPIOD, GPIO_Pin_2);
    uint32_t addr = 0;
    uint16_t i    = 0;
    Keyboard_IO_Init();
    SPI_FLASH_Init();   //flash�ĳ�ʼ��
    LCD_Init();         //LCD��ʼ��
////    EXTI0_INT_INIT();   //��������ʼ��
    System_Init();
//    ADC1_Init();
    LCD_drawImageWithSize(20, 0, LCD_W, LCD_H, 134400);

   // led(4);

	while( 1 )
    {
	    System_Loop();

//	    LCD_ShowChinese(0,0,"�о�԰����",RED,WHITE,32,0);
//	    LCD_ShowString(0,40,"LCD_W:",RED,WHITE,16,0);
	    KB_Scan_Handle();//�Զ�ѡ�����ߣ�����
	   // LCD_DisplayAnimation();

    }
}
