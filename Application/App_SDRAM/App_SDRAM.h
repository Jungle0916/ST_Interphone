/********************************************************
Copyright (c) 2025 Year, Dongguan City University., Ltd. All rights reserved.
FileName  : App_SDRAM.h 
Author    : zheng_kai_wen 
Version   : v0.0.0.0 
Date      : 2025.09.26 
Note      : none
History   : none 
*********************************************************/
#ifndef __APP_SDRAM_H
#define	__APP_SDRAM_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "usart.h"
/* Public define ------------------------------------------------*/
#define SDRAM_Size 0x02000000  //32M字节
#define SDRAM_BANK_ADDR     ((uint32_t)0xD0000000) 				// FMC SDRAM 数据基地址
#define FMC_COMMAND_TARGET_BANK   FMC_SDRAM_CMD_TARGET_BANK2	//	SDRAM 的bank选择
#define SDRAM_TIMEOUT     ((uint32_t)0x1000) 						// 超时判断时间

#define SDRAM_MODEREG_BURST_LENGTH_1             ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_LENGTH_2             ((uint16_t)0x0001)
#define SDRAM_MODEREG_BURST_LENGTH_4             ((uint16_t)0x0002)
#define SDRAM_MODEREG_BURST_LENGTH_8             ((uint16_t)0x0004)
#define SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL      ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_TYPE_INTERLEAVED     ((uint16_t)0x0008)
#define SDRAM_MODEREG_CAS_LATENCY_2              ((uint16_t)0x0020)
#define SDRAM_MODEREG_CAS_LATENCY_3              ((uint16_t)0x0030)
#define SDRAM_MODEREG_OPERATING_MODE_STANDARD    ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_PROGRAMMED ((uint16_t)0x0000) 
#define SDRAM_MODEREG_WRITEBURST_MODE_SINGLE     ((uint16_t)0x0200) 
/* Private typedef -----------------------------------------------*/

/* Public variables ----------------------------------------------*/

/* Public function prototypes ------------------------------------*/
uint8_t 	SDRAM_Test(void);     		// SDRAM测试函数
void SDRAM_Initialization_Sequence(SDRAM_HandleTypeDef *hsdram, FMC_SDRAM_CommandTypeDef *Command);

#ifdef __cplusplus
}
#endif

#endif 
