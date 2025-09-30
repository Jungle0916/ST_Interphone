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
 * description  : �ض���printf���� ʹprintf�������������USART1
 * Input        : ch: Ҫ������ַ�
 * Output       : ����USART1���ch�ַ�
 * Return       : none
 ********************************************************************/
int fputc(int ch, FILE *file)
{
    // ��printf�����ض��� ʹ�ô���USART1���������
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xffff);
    return ch;
}
