/********************************************************
Copyright (c) 2025 Year, Dongguan City University., Ltd. All rights reserved.
FileName  : Task_Peripheral_Init.h 
Author    : zheng_kai_wen 
Version   : v0.0.0.0 
Date      : 2025.09.28 
Note      : none
History   : none 
*********************************************************/
#ifndef __TASK_PERIPHERAL_INIT_H
#define	__TASK_PERIPHERAL_INIT_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------*/
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "fatfs.h"
#include "ff.h"
#include "bsp_driver_sd.h"
#include "sd_diskio.h"
#include "stdio.h"

/* Public define ------------------------------------------------*/
/* Private typedef -----------------------------------------------*/

/* Public variables ----------------------------------------------*/

/* Public function prototypes ------------------------------------*/
void Task_Peripheral_Init(void *argument);

#ifdef __cplusplus
}
#endif

#endif
