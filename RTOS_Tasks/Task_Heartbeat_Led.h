/********************************************************
Copyright (c) 2025 Dongguan City University. All rights reserved.
FileName  : Task_Heartbeat_Led.h 
Author    : zheng_kai_wen 
Version   : v0.0.0.0 
Date      : 2025.09.26 
Note      : none
History   : none 
*********************************************************/
#ifndef __TASK_HEARTBEAT_LED_H
#define	__TASK_HEARTBEAT_LED_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------*/
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Public define ------------------------------------------------*/
/* Private typedef -----------------------------------------------*/

/* Public variables ----------------------------------------------*/

/* Public function prototypes ------------------------------------*/
void Task_Heartbeat_Led(void *argument);

#ifdef __cplusplus
}
#endif

#endif
