/********************************************************
Copyright (c) 2025 Year, Dongguan City University., Ltd. All rights reserved.
FileName  : Com_Debug.h
Author    : zheng_kai_wen 
Version   : v0.0.0.0 
Date      : 2025.09.26 
Note      : none
History   : none 
*********************************************************/
#ifndef __COM_DEBUG__
#define __COM_DEBUG__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------*/
#include "usart.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
/* Public define ------------------------------------------------*/
/* �������ԵĹ�����  ������ӡ���  һ����Ŀ���� ���� �رյ����еĴ������ */
#define DEBUG_ENABLE

#ifdef DEBUG_ENABLE
// �滻__FILE__ ֻ�����ļ�����  ȡ��·������
// FILE__NAME => ȥ��\\��·������
#define FILE__NAME (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

// ���յ���__FILE__NAME__  => ȥ������/·������  �ڲ�����FILE__NAME
#define __FILE__NAME__ (strrchr(FILE__NAME, '/') ? strrchr(FILE__NAME, '/') + 1 : FILE__NAME)

#define debug_printf(format, ...) printf("[%s:%d]" format, __FILE__NAME__, __LINE__, ##__VA_ARGS__)
#define debug_println(format, ...) printf("[%s:%d]" format "\r\n", __FILE__NAME__, __LINE__, ##__VA_ARGS__)

// ʵ�ֿ������ڴ�ӡ�ĺ궨��
#else
// �յĺ궨�� => ����ͨ��
#define FILE__NAME
#define __FILE__NAME__
#define debug_printf(format, ...)
#define debug_println(format, ...)

#endif

/* Private typedef -----------------------------------------------*/

/* Public variables ----------------------------------------------*/

/* Public function prototypes ------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif
