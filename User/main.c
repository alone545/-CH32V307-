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

    CH32_PA9   -- ESP P9（ RX）
    CH32_PA10  -- ESP P10（TX）

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
//    // 使能 GPIOA 时钟
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
//    // 使能 USART2 时钟（USART2 位于 APB1 总线上）
//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
//
//    // 配置 PA2 为复用推挽 (TX)
//    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
//    GPIO_Init(GPIOA, &GPIO_InitStructure);
//
//    // 配置 PA3 为浮空输入 (RX)
//    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//    GPIO_Init(GPIOA, &GPIO_InitStructure);
//
//    // USART2 参数配置
//    USART_InitStructure.USART_BaudRate            = baud;
//    USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
//    USART_InitStructure.USART_StopBits            = USART_StopBits_1;
//    USART_InitStructure.USART_Parity              = USART_Parity_No;
//    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//    USART_InitStructure.USART_Mode                = USART_Mode_Tx | USART_Mode_Rx;
//    USART_Init(USART2, &USART_InitStructure);
//
//    // 使能 USART2
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

    // 使能 GPIOA 时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    // 使能 USART2 时钟（USART2 位于 APB1 总线上）
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

    // 配置 PA2 为复用推挽 (TX)
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 配置 PA3 为浮空输入 (RX)
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // USART2 参数配置
    USART_InitStructure.USART_BaudRate            = baud;
    USART_InitStructure.USART_WordLength          = USART_WordLength_8b;  // 8 数据位
    USART_InitStructure.USART_StopBits            = USART_StopBits_1;      // 1 停止位
    USART_InitStructure.USART_Parity              = USART_Parity_No;     // 使用偶校验位，改为 USART_Parity_Odd 使用奇校验
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode                = USART_Mode_Tx | USART_Mode_Rx;

    // 初始化 USART2
    USART_Init(USART2, &USART_InitStructure);

    // 使能 USART2
    USART_Cmd(USART2, ENABLE);
}

// LCD 动画显示函数，放入主循环或任务中
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
    SPI_FLASH_Init();   //flash的初始化
    LCD_Init();         //LCD初始化
////    EXTI0_INT_INIT();   //编码器初始化
    System_Init();
//    ADC1_Init();
    LCD_drawImageWithSize(20, 0, LCD_W, LCD_H, 134400);

   // led(4);

	while( 1 )
    {
	    System_Loop();

//	    LCD_ShowChinese(0,0,"中景园电子",RED,WHITE,32,0);
//	    LCD_ShowString(0,40,"LCD_W:",RED,WHITE,16,0);
	    KB_Scan_Handle();//自动选择有线，蓝牙
	   // LCD_DisplayAnimation();

    }
}
