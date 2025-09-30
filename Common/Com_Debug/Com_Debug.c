/********************************************************
Copyright (c) 2025 Dongguan City University. All rights reserved.
FileName  : Com_Debug.c 
Author    : zheng_kai_wen 
Version   : v0.0.0.0 
Date      : 2025.09.26 
Note      : none
History   : none 
*********************************************************/
/* Includes ------------------------------------------------------*/
#include "Com_Debug.h"

/* Private define ------------------------------------------------*/
/* Private typedef -----------------------------------------------*/
/* Public variables ----------------------------------------------*/

/* Public function prototypes ------------------------------------*/
/**
 ********************************************************************
 * name         : int fputc(int ch, FILE *file)
 * description  : 重定向printf方法 使printf方法输出到串口USART1
 * Input        : ch: 要输出的字符
 * Output       : 串口USART1输出ch字符
 * Return       : none
 ********************************************************************/
int fputc(int ch, FILE *file)
{
    // 让printf方法重定向 使用串口USART1输出到电脑
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xffff);
    return ch;
}
