/********************************************************
Copyright (c) 2025 Dongguan City University. All rights reserved.
FileName  : Int_Touch.h 
Author    : zheng_kai_wen 
Version   : v0.0.0.0 
Date      : 2025.09.29
Note      : 本代码参考反客科技,但是将软件IIC改成了硬件IIC2
History   : none 
*********************************************************/
/* Includes ------------------------------------------------------*/
#ifndef __INT_TOUCH_H
#define __INT_TOUCH_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------*/
#include "main.h"
#include "i2c.h"

/* Public define ------------------------------------------------*/  	
#define TOUCH_MAX   5	//最大触摸点数

#define GT9XX_IIC_RADDR 0x29			// IIC读地址
#define GT9XX_IIC_WADDR 0x28			// IIC写地址

#define GT9XX_READ_ADDR 0x814E		// 读取信息寄存器地址
#define GT9XX_ID_ADDR 	0x8140		// 芯片ID寄存器地址

/* Private typedef -----------------------------------------------*/  		
typedef struct 
{
	uint8_t  flag;			// 触摸标志位，为1时表示有触摸事件
	uint8_t  num;				// 触摸点数
	uint16_t x[TOUCH_MAX];	// x坐标
	uint16_t y[TOUCH_MAX];	// y坐标
}TouchStructure;

/* Public variables ----------------------------------------------*/
// 声明外部变量
extern TouchStructure touchInfo;

/* Public function prototypes ------------------------------------*/
uint8_t Touch_Init(void);		// 触摸屏初始化
void Touch_Scan(void);		    // 触摸扫描
void GT9XX_Reset(void);	    // 执行复位操作

#ifdef __cplusplus
}
#endif

#endif
