#ifndef __SYSTEM_H
#define __SYSTEM_H

#ifdef __cplusplus
 extern "C" {
#endif

//============================��ϵͳ��ʼ����================================
void System_Init();

//==============================����ѭ����=================================
void System_1();
extern void breathing_effect(int row_index);
//=============================��������=================================
void System_Error_Handler();
extern float brightness_factor ;
#ifdef __cplusplus
 }
#endif

#endif /* __SYSTEM_H */


