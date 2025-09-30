/********************************************************
Copyright (c) 2025 Dongguan City University. All rights reserved.
FileName  : Int_Touch.h 
Author    : zheng_kai_wen 
Version   : v0.0.0.0 
Date      : 2025.09.29
Note      : ������ο����ͿƼ�,���ǽ����IIC�ĳ���Ӳ��IIC2
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
#define TOUCH_MAX   5	//���������

#define GT9XX_IIC_RADDR 0x29			// IIC����ַ
#define GT9XX_IIC_WADDR 0x28			// IICд��ַ

#define GT9XX_READ_ADDR 0x814E		// ��ȡ��Ϣ�Ĵ�����ַ
#define GT9XX_ID_ADDR 	0x8140		// оƬID�Ĵ�����ַ

/* Private typedef -----------------------------------------------*/  		
typedef struct 
{
	uint8_t  flag;			// ������־λ��Ϊ1ʱ��ʾ�д����¼�
	uint8_t  num;				// ��������
	uint16_t x[TOUCH_MAX];	// x����
	uint16_t y[TOUCH_MAX];	// y����
}TouchStructure;

/* Public variables ----------------------------------------------*/
// �����ⲿ����
extern TouchStructure touchInfo;

/* Public function prototypes ------------------------------------*/
uint8_t Touch_Init(void);		// ��������ʼ��
void Touch_Scan(void);		    // ����ɨ��
void GT9XX_Reset(void);	    // ִ�и�λ����

#ifdef __cplusplus
}
#endif

#endif
