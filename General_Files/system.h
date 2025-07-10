#ifndef __SYSTEM_H
#define __SYSTEM_H

#ifdef __cplusplus
 extern "C" {
#endif

//============================【系统初始化】================================
void System_Init();

//==============================【主循环】=================================
void System_1();
extern void breathing_effect(int row_index);
//=============================【错误处理】=================================
void System_Error_Handler();
extern float brightness_factor ;
#ifdef __cplusplus
 }
#endif

#endif /* __SYSTEM_H */


