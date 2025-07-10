
/*******************************************************************************/
/* Header Files */
#include "ch32v30x_usbfs_device.h"
#include "usbd_composite_km.h"
#include "stdio.h"
#include "ch32v30x_gpio.h"
#include "ch32v30x_rcc.h"
#include "system.h"
#include "WS2812_PWM_drv.h"

static void UART1_SendReport(uint8_t *report_ptr)
{
    for (uint8_t i = 0; i < 8; i++) {
        USART_SendData(USART1, report_ptr[i]);
        while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET) {}
    }
}
//static void UART2_SendReport(uint8_t *report_ptr)
//{
//    for (uint8_t i = 0; i < 8; i++) {
//        USART_SendData(USART2, report_ptr[i]);
//        while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET) {}
//    }
//}
// 修改 UART2_SendReport 函数，添加帧头
static void UART2_SendReport(uint8_t *report_ptr)
{
    // 先发送帧头 0x80
    USART_SendData(USART2, 0x80);
    while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET) {}

    // 再发送8字节的报告
    for (uint8_t i = 0; i < 8; i++) {
        USART_SendData(USART2, report_ptr[i]);
        while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET) {}
    }
}
// 按键状态数组
uint8_t key_state[ROW_NUM * COL_NUM] = {0};

// 键盘矩阵定义（包括无键位置）
uint8_t key_matrix[ROW_NUM][COL_NUM] = {
    {ESC, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, PRTSC, DELETE, NO_KEY, NO_KEY, NO_KEY},  // 第 0 行
    {BACKTICK, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9, KEY_0, MINUS, EQUALS, BACKSPACE, NO_KEY, NO_KEY, NO_KEY, NO_KEY},  // 第 1 行
    {TAB, NO_KEY, Q, W, E, R, T, Y, U, I, O, P, LEFT_BRACKET, RIGHT_BRACKET, BACKSLASH, NO_KEY, NO_KEY, NO_KEY},  // 第 2 行
    {CAPS_LOCK, NO_KEY, A, S, D, F, G, H, J, K, L, SEMICOLON, QUOTE, ENTER, NO_KEY, NO_KEY, NO_KEY, NO_KEY},  // 第 3 行
    {LEFT_SHIFT, NO_KEY, Z, X, C, V, B, N, M, COMMA, PERIOD, SLASH,NO_KEY, RIGHT_SHIFT, NO_KEY, NO_KEY, UP_ARROW, NO_KEY},  // 第 4 行
    {LEFT_CTRL, NO_KEY, WIN, LEFT_ALT, NO_KEY, NO_KEY , NO_KEY, SPACE, NO_KEY, NO_KEY, NO_KEY,RIGHT_ALT, NO_KEY, FN,RIGHT_CTRL,LEFT_ARROW, DOWN_ARROW, RIGHT_ARROW}   // 第 5 行
};
// 行引脚定义（PD0 到 PD5）
uint16_t row_pins[ROW_NUM] = {GPIO_Pin_0, GPIO_Pin_1, GPIO_Pin_2, GPIO_Pin_3, GPIO_Pin_4, GPIO_Pin_5};

// 列引脚定义（PE7 到 PE15, PD8 到 PD15, PD6）
uint16_t col_pins[COL_NUM] = {
    GPIO_Pin_7, GPIO_Pin_8, GPIO_Pin_9, GPIO_Pin_10, GPIO_Pin_11, GPIO_Pin_12,
    GPIO_Pin_13, GPIO_Pin_14, GPIO_Pin_15, GPIO_Pin_8, GPIO_Pin_9, GPIO_Pin_10,
    GPIO_Pin_11, GPIO_Pin_12, GPIO_Pin_13, GPIO_Pin_14, GPIO_Pin_15, GPIO_Pin_6  // PD6
};
// 按键扫描：初始化行和列的 IO 口
void Keyboard_IO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* 使能 PD、PE 时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE, ENABLE);

    /* --- 行：PD0 到 PD5 做开漏输出，初始拉高 --- */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  // 开漏输出
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    /* 空闲态拉高 */
    GPIO_SetBits(GPIOD, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5);

    /* --- 列：PE7 到 PE15 做上拉输入 --- */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  // 上拉输入
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    /* --- 列：PD6 到 PD15 做上拉输入 --- */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  // 上拉输入
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;

     GPIO_Init(GPIOD, &GPIO_InitStructure);
     GPIO_ResetBits(GPIOD, GPIO_Pin_7);
}

static uint32_t Read_Columns(void)
{
    uint32_t col_mask = 0;

    // 读取前 9 列：PE7 到 PE15
    for (uint32_t i = 0; i < 9; i++) {
        if (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_7 << i) == Bit_RESET) {
            col_mask |= (1 << i);  // 按键按下，列对应位设置为 1
        }
    }

    // 读取第 10 到第 17 列：PD8 到 PD15
    for (uint32_t i = 0; i < 8; i++) {
        if (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_8 << i) == Bit_RESET) {
            col_mask |= (1 << (i + 9));  // 按键按下，列对应位设置为 1
        }
    }

    // 读取第 18 列：PD6
    if (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_6) == Bit_RESET) {
        col_mask |= (1 << 17);  // 第 18 列按下
    }

    return col_mask;
}
static void Drive_Row_Low(uint8_t row_idx)
{
    // 确保只有当前行被拉低
    for (uint8_t i = 0; i < 6; i++) {
        if (i == row_idx)
            GPIO_ResetBits(GPIOD, GPIO_Pin_0 << i);  // 拉低当前行
        else
            GPIO_SetBits(GPIOD, GPIO_Pin_0 << i);    // 拉高其他行
    }
}



void KB_Scan(void)
{
    uint8_t row;
    uint32_t  col_mask;
    // 扫描每一行
    for (row = 0; row < 6; row++) {
        // 拉低当前行，其他行拉高
        Drive_Row_Low(row);

        // 延时让信号稳定
        for (volatile int i = 0; i < 100; i++);

        // 读取列状态
        col_mask = Read_Columns();

        // 更新按键状态
        for (uint32_t c = 0; c < 18; c++){
            uint32_t idx = row * 18 + c;  // 根据行列计算按键的索引

            // 如果按键位置不是 NO_KEY 且按键被按下，更新状态
            if (key_matrix[row][c] != NO_KEY) {
                key_state[idx] = (col_mask & (1 << c)) ? 1 : 0;  // 如果按键按下，则为 1
            }
        }
    }

    // 拉高所有行，避免浮空状态
    GPIO_SetBits(GPIOD, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5);
}


//void KB_Scan_Handle(void)
//{
//    uint8_t modifier = 0;  // 修饰键状态
//    uint8_t keybuf[6];     // 存储最多 6 个普通按键
//    uint8_t key_count = 0;
//
//    // 填充 keybuf[] 数组（最多 6 个按键）
//    for (uint8_t idx = 0; idx < ROW_NUM * COL_NUM; idx++)
//    {
//        if(key_state[idx]&& key_matrix[idx / 18][idx % 18] ==LEFT_SHIFT)
//        {
//            modifier=LEFT_SHIFT;
//            //modifier = LEFT_SHIFT;
//        }
//       else if (key_state[idx]&& key_matrix[idx / 18][idx % 18] ==LEFT_CTRL)
//       {
//           modifier = LEFT_CTRL ;
//    }
//       else if (key_state[idx]&& key_matrix[idx / 18][idx % 18] ==LEFT_ALT)
//             {
//                 modifier = LEFT_ALT;
//          }
//       else if (key_state[idx]&& key_matrix[idx / 18][idx % 18] ==RIGHT_CTRL)
//             {
//                 modifier = RIGHT_CTRL;
//          }
//       else if (key_state[idx]&& key_matrix[idx / 18][idx % 18] ==RIGHT_SHIFT)
//             {
//                 modifier = RIGHT_SHIFT;
//          }
//       else if (key_state[idx]&& key_matrix[idx / 18][idx % 18] ==WIN)
//             {
//                 modifier = WIN;
//          }
//       else if (key_state[idx]&& key_matrix[idx / 18][idx % 18] ==FN)
//             {
//                 modifier = FN;
//          }
//        if (key_state[idx] && key_matrix[idx / 18][idx % 18] != NO_KEY&&key_state[idx]&& key_matrix[idx / 18][idx % 18] !=LEFT_CTRL&& key_matrix[idx / 18][idx % 18] !=LEFT_SHIFT
//                && key_matrix[idx / 18][idx % 18] !=RIGHT_CTRL&& key_matrix[idx / 18][idx % 18] !=RIGHT_SHIFT&& key_matrix[idx / 18][idx % 18] !=RIGHT_ALT
//                && key_matrix[idx / 18][idx % 18] !=WIN&& key_matrix[idx / 18][idx % 18] !=FN)
//        {
//            if (key_count < 6)
//            {
//                keybuf[key_count++] = key_matrix[idx / 18][idx % 18];
//            }
//        }
//    }
//
//    // 如果按键少于 6 个，填充 NO_KEY
//    while (key_count < 6)
//    {
//        keybuf[key_count++] = NO_KEY;
//    }
//
//    // 生成 HID 报文
//    extern uint8_t KB_Data_Pack[8];
//    KB_Data_Pack[0] =modifier;  // 修饰键字节
//    KB_Data_Pack[1] = 0x00;      // 保留字节
//
//    for (uint8_t i = 0; i < 6; i++) {
//        KB_Data_Pack[2 + i] = keybuf[i];  // 填充普通按键
//    }
//
//    // 发送 HID 报文
//    if (USBFS_DevEnumStatus)
//    {
//
//        USBFS_Endp_DataUp(DEF_UEP1, KB_Data_Pack, sizeof(KB_Data_Pack), DEF_UEP_CPY_LOAD);
//    }
//    UART2_SendReport(KB_Data_Pack);
//}


// 添加全局变量用于跟踪按键状态和重复计数
static uint8_t last_report[8] = {0};
static uint8_t repeat_counter = 0;
#define KEY_REPEAT_DELAY 75  // 重复延迟（扫描次数）
void KB_Scan_Handle(void)
{
    uint8_t modifier = 0;  // 修饰键状态
    uint8_t keybuf[6];     //

    uint8_t key_count = 0;

    // 填充 keybuf[]


    for (uint8_t idx = 0; idx < ROW_NUM * COL_NUM; idx++)
    {
        if(key_state[idx]&& key_matrix[idx / 18][idx % 18] ==LEFT_SHIFT)
        {
            modifier=LEFT_SHIFT;
            System_1();
//            breathing_effect(0); 阿斯蒂芬规划局快乐234567890

//            WS2812_SendData(color1);

        }
       else if (key_state[idx]&& key_matrix[idx / 18][idx % 18] ==LEFT_CTRL)
       {
           modifier = LEFT_CTRL ;
           //System_1();

    }
       else if (key_state[idx]&& key_matrix[idx / 18][idx % 18] ==LEFT_ALT)
             {
                 modifier = LEFT_ALT;
          }
       else if (key_state[idx]&& key_matrix[idx / 18][idx % 18] ==RIGHT_CTRL)
             {
                 modifier = RIGHT_CTRL;
          }
       else if (key_state[idx]&& key_matrix[idx / 18][idx % 18] ==RIGHT_SHIFT)
             {
                 modifier = RIGHT_SHIFT;
          }
       else if (key_state[idx]&& key_matrix[idx / 18][idx % 18] ==WIN)
             {
                 modifier = WIN;
          }
       else if (key_state[idx]&& key_matrix[idx / 18][idx % 18] ==FN)
             {
                 modifier = FN;
          }
        if (key_state[idx] && key_matrix[idx / 18][idx % 18] != NO_KEY&&key_state[idx]&& key_matrix[idx / 18][idx % 18] !=LEFT_CTRL&& key_matrix[idx / 18][idx % 18] !=LEFT_SHIFT
                && key_matrix[idx / 18][idx % 18] !=RIGHT_CTRL&& key_matrix[idx / 18][idx % 18] !=RIGHT_SHIFT&& key_matrix[idx / 18][idx % 18] !=RIGHT_ALT
                && key_matrix[idx / 18][idx % 18] !=WIN&& key_matrix[idx / 18][idx % 18] !=FN)
        {
            if (key_count < 6)
            {
                keybuf[key_count++] = key_matrix[idx / 18][idx % 18];
            }
        }
    }

    // 如果按键少于 6 个，填充 NO_KEY
    while (key_count < 6)
    {
        keybuf[key_count++] = NO_KEY;
    }
    // 生成 HID 报文
    // ... [原有的扫描和组包代码保持不变] ...

    // 生成 HID 报文
    extern uint8_t KB_Data_Pack[8];
    KB_Data_Pack[0] = modifier;  // 修饰键字节
    KB_Data_Pack[1] = 0x00;      // 保留字节

    for (uint8_t i = 0; i < 6; i++) {
        KB_Data_Pack[2 + i] = keybuf[i];  // 填充普通按键
    }

    // 检查报告是否有变化
    uint8_t report_changed = memcmp(KB_Data_Pack, last_report, 8) != 0;

    // 检查是否有按键按下（非全零报告）
    uint8_t has_key = 0;
    for (int i = 0; i < 8; i++) {
        if (KB_Data_Pack[i] != 0) {
            has_key = 1;
            break;
        }
    }

    // 发送逻辑：
    // 1. 报告变化时立即发送
    // 2. 有按键按下且达到重复计数时发送
    if (report_changed || (has_key && repeat_counter >= KEY_REPEAT_DELAY)) {
        memcpy(last_report, KB_Data_Pack, 8);

        // 发送 HID 报文
        if (USBFS_DevEnumStatus) {
            USBFS_Endp_DataUp(DEF_UEP1, KB_Data_Pack, sizeof(KB_Data_Pack), DEF_UEP_CPY_LOAD);
        }
        UART2_SendReport(KB_Data_Pack);

        // 重置重复计数器
        repeat_counter = 0;
    }

    // 如果按键持续按下，增加重复计数器
    if (has_key) {
        repeat_counter++;
    } else {
        repeat_counter = 0;
    }
}











/*******************************************************************************/
/* Variable Definition */


/* Keyboard */
volatile uint8_t  KB_Scan_Done = 0x00;                                          // Keyboard Keys Scan Done
volatile uint16_t KB_Scan_Result = 0xF000;                                      // Keyboard Keys Current Scan Result
volatile uint16_t KB_Scan_Last_Result = 0xF000;                                 // Keyboard Keys Last Scan Result
uint8_t  KB_Data_Pack[ 8 ] = { 0x00 };                                          // Keyboard IN Data Packet
volatile uint8_t  KB_LED_Last_Status = 0x00;                                    // Keyboard LED Last Result
volatile uint8_t  KB_LED_Cur_Status = 0x00;                                     // Keyboard LED Current Result


/*******************************************************************************/
/* Interrupt Function Declaration */
void TIM3_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));


/*********************************************************************
 * @fn      TIM3_Init
 *
 * @brief   Initialize timer3 for keyboard and mouse scan.
 *
 * @param   arr - The specific period value
 *          psc - The specifies prescaler value
 *
 * @return  none
 */
void TIM3_Init( uint16_t arr, uint16_t psc )
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure = { 0 };
    NVIC_InitTypeDef NVIC_InitStructure = { 0 };

    RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM3, ENABLE );

    TIM_TimeBaseStructure.TIM_Period = arr;
    TIM_TimeBaseStructure.TIM_Prescaler = psc;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit( TIM3, &TIM_TimeBaseStructure );

    TIM_ITConfig( TIM3, TIM_IT_Update, ENABLE );

    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init( &NVIC_InitStructure );

    TIM_Cmd( TIM3, ENABLE );
}

/*********************************************************************
 * @fn      TIM3_IRQHandler
 *
 * @brief   This function handles TIM3 global interrupt request.
 *
 * @return  none
 */
// 添加全局时间计数器
volatile uint32_t system_time_ms = 0;
volatile uint32_t delay_counter = 0; // 延时计数器
void TIM3_IRQHandler( void )
{
    if( TIM_GetITStatus( TIM3, TIM_IT_Update ) != RESET )
    {
        /* Clear interrupt flag */
        TIM_ClearITPendingBit( TIM3, TIM_IT_Update );

        /* Handle keyboard scan */
        KB_Scan( );
        system_time_ms++;
    }
}
// 在主循环中使用定时器计数来代替阻塞延时
void de_mss(uint32_t ms)
{
    uint32_t start_time = delay_counter;
    while (delay_counter - start_time < ms);
}
/*********************************************************************
 * @fn      KB_Sleep_Wakeup_Cfg
 *
 * @brief   Configure keyboard wake up mode.
 *
 * @return  none
 */
void KB_Sleep_Wakeup_Cfg( void )
{
    EXTI_InitTypeDef EXTI_InitStructure = { 0 };

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIO, ENABLE );

    GPIO_EXTILineConfig( GPIO_PortSourceGPIOB, GPIO_PinSource12 );
    EXTI_InitStructure.EXTI_Line = EXTI_Line12;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Event;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init( &EXTI_InitStructure );

    GPIO_EXTILineConfig( GPIO_PortSourceGPIOB, GPIO_PinSource13 );
    EXTI_InitStructure.EXTI_Line = EXTI_Line13;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Event;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init( &EXTI_InitStructure );

    GPIO_EXTILineConfig( GPIO_PortSourceGPIOB, GPIO_PinSource14 );
    EXTI_InitStructure.EXTI_Line = EXTI_Line14;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Event;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init( &EXTI_InitStructure );

    GPIO_EXTILineConfig( GPIO_PortSourceGPIOB, GPIO_PinSource15 );
    EXTI_InitStructure.EXTI_Line = EXTI_Line15;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Event;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init( &EXTI_InitStructure );

    EXTI->INTENR |= EXTI_INTENR_MR12 | EXTI_INTENR_MR13 | EXTI_INTENR_MR14 | EXTI_INTENR_MR15;
}


/**
 * @brief  调用扫描并组 HID 报文发送
 */
/**
 * @brief  Handle matrix results, build HID report and send over USB
 */

/*
 * KB_Scan_Handle()
 * ----------------------------------------------------------------------------
 * 不再内部调用 KB_Scan()，仅根据全局 key_state[] 组装 HID 报文并上传到主机。
 * HID 报文 8 字节：
 *   Byte0：Modifier (bit0 = 左 Ctrl, bit1 = 左 Shift,...)
 *   Byte1：Reserved (0x00)
 *   Byte2~7：最多 6 个 KeyCode，空位填 HID_KEY_NONE(0x00)。
 */


/*********************************************************************
 * @fn      USB_Sleep_Wakeup_CFG
 *
 * @brief   Configure USB wake up mode
 *
 * @return  none
 */
void USB_Sleep_Wakeup_CFG( void )
{
    EXTI_InitTypeDef EXTI_InitStructure = { 0 };

    EXTI_InitStructure.EXTI_Line = EXTI_Line18;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Event;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init( &EXTI_InitStructure );
}

/*********************************************************************
 * @fn      MCU_Sleep_Wakeup_Operate
 *
 * @brief   Perform sleep operation
 *
 * @return  none
 */
void MCU_Sleep_Wakeup_Operate( void )
{

    __disable_irq( );
    EXTI_ClearFlag( EXTI_Line12 | EXTI_Line13 | EXTI_Line14 | EXTI_Line15 );
    EXTI_ClearFlag( EXTI_Line4 | EXTI_Line5 | EXTI_Line6 | EXTI_Line7 );

    PWR_EnterSTOPMode(PWR_Regulator_LowPower,PWR_STOPEntry_WFE);

    SystemInit();
    SystemCoreClockUpdate();
    USBFS_RCC_Init();

    if( EXTI_GetFlagStatus( EXTI_Line12 | EXTI_Line13 | EXTI_Line14 | EXTI_Line15 ) != RESET  )
    {
        EXTI_ClearFlag( EXTI_Line12 | EXTI_Line13 | EXTI_Line14 | EXTI_Line15 );
        USBFS_Send_Resume( );
    }
    else if( EXTI_GetFlagStatus( EXTI_Line4 | EXTI_Line5 | EXTI_Line6 | EXTI_Line7 ) != RESET )
    {
        EXTI_ClearFlag( EXTI_Line4 | EXTI_Line5 | EXTI_Line6 | EXTI_Line7 );
        USBFS_Send_Resume( );
    }
    __enable_irq( );

}



