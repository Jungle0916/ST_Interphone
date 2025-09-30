/********************************************************
Copyright (c) 2025 Dongguan City University. All rights reserved.
FileName  : Task_Heartbeat_Led.c 
Author    : zheng_kai_wen 
Version   : v0.0.0.0 
Date      : 2025.09.26 
Note      : LED任务仅用于验证程序是否卡死,无其他用途
History   : none 
*********************************************************/
/* Includes ------------------------------------------------------*/
#include "Task_Heartbeat_Led.h"
#include "Int_LED.h" 

/* Private define ------------------------------------------------*/
/* Private typedef -----------------------------------------------*/
/* Public variables ----------------------------------------------*/

/* Public function prototypes ------------------------------------*/
/**
 ********************************************************************
 * name         : void Task_Heartbeat_Led(void *argument)
 * description  : LED每500ms切换一次状态,用于验证程序是否卡死
 * Input        : none
 * Output       : LED1_PIN状态切换
 * Return       : none
 ********************************************************************/
void Task_Heartbeat_Led(void *argument)
{
	for(;;)
	{
		LED_Toggle(LED1_PIN);
		osDelay(500);
	}
}
