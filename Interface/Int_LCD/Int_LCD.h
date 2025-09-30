/********************************************************
Copyright (c) 2025 Dongguan City University. All rights reserved.
FileName  : Int_LCD.h 
Author    : zheng_kai_wen 
Version   : v0.0.0.0 
Date      : 2025.09.29 
Note      : ������ο����ͿƼ�,��ɾ����ԭ����ͼ��,������ʾ����
History   : none 
*********************************************************/
#ifndef __INT_LCD_H
#define	__INT_LCD_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------*/
#include "main.h"

/* Private define ------------------------------------------------*/
#define LCD_Width     	800				//	LCD�����س���
#define LCD_Height    	480				//	LCD�����ؿ��
#define LCD_MemoryAdd   0xD0000000 		//	�Դ����ʼ��ַ  

#define	Direction_H	0		//LCD������ʾ
#define	Direction_V	1		//LCD������ʾ

#define  Fill_Zero  0		//���0
#define  Fill_Space 1		//���ո�

/*---------------------------------------- ������ɫ ------------------------------------------------------

 1. ����Ϊ�˷����û�ʹ�ã��������32λ��ɫ��Ȼ����ͨ�������Զ�ת���ɶ�Ӧ��ɫ��ʽ����Ҫ�ĵ���ɫ
 2. 32λ����ɫ�У��Ӹ�λ����λ�ֱ��Ӧ A��R��G��B  4����ɫͨ��������A��ʾ͸��ͨ��
 3. ��������255��͸��ɫ��ff��ʾ��͸����0��ʾ��ȫ͸��
 4. ����ʹ��ARGB1555��ARGB8888��֧��͸��ɫ����ɫ��ʽ����Ȼ͸��ɫ�������ã�����ARGB1555��֧��һλ
	 ͸��ɫ��������͸���Ͳ�͸������״̬��ARGB4444��16��͸���ȣ�ARGB8888֧��255��͸����
 5. �û������ڵ����õ�ɫ���ȡ24λRGB��ɫ��Ȼ���ٲ���͸��ͨ���õ�32λ����ɫ���ٽ���32λ��ɫ����
	 LCD_SetColor()��LCD_SetBackColor()�Ϳ�����ʾ����Ӧ����ɫ�� 
 6. ʹ��ʾ��������ɫ��RGBֵΪ0x0000FF���������Ҫ͸��ɫ�����Ӧ��32λ��ɫֵΪ 0xff0000FF
 7. ���¶������ɫ������Ϊ��͸�����û��ɸ����������ж����Ӧ����ɫ								*/

#define 	LCD_WHITE       0xffFFFFFF		// ����ɫ
#define 	LCD_BLACK       0xff000000		// ����ɫ
                           
#define 	LCD_BLUE        0xff0000FF		//	����ɫ
#define 	LCD_GREEN       0xff00FF00    //	����ɫ
#define 	LCD_RED         0xffFF0000    //	����ɫ
#define 	LCD_CYAN        0xff00FFFF    //	����ɫ
#define 	LCD_MAGENTA     0xffFF00FF    //	�Ϻ�ɫ
#define 	LCD_YELLOW      0xffFFFF00    //	��ɫ
#define 	LCD_GREY        0xff2C2C2C    //	��ɫ
													
#define 	LIGHT_BLUE      0xff8080FF    //	����ɫ
#define 	LIGHT_GREEN     0xff80FF80    //	����ɫ
#define 	LIGHT_RED       0xffFF8080    //	����ɫ
#define 	LIGHT_CYAN      0xff80FFFF    //	������ɫ
#define 	LIGHT_MAGENTA   0xffFF80FF    //	���Ϻ�ɫ
#define 	LIGHT_YELLOW    0xffFFFF80    //	����ɫ
#define 	LIGHT_GREY      0xffA3A3A3    //	����ɫ
													
#define 	DARK_BLUE       0xff000080    //	����ɫ
#define 	DARK_GREEN      0xff008000    //	����ɫ
#define 	DARK_RED        0xff800000    //	����ɫ
#define 	DARK_CYAN       0xff008080    //	������ɫ
#define 	DARK_MAGENTA    0xff800080    //	���Ϻ�ɫ
#define 	DARK_YELLOW     0xff808000    //	����ɫ
#define 	DARK_GREY       0xff404040    //	����ɫ

/* Private typedef -----------------------------------------------*/
/* Public variables ----------------------------------------------*/
void  LTDC_Init(void);  //LCD��ʼ��
void  LCD_Clear(void); //����
void  LCD_ClearRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height);	// �ֲ���������

void  LCD_SetLayer(uint8_t Layerx); 					//	���ò�
void  LCD_SetColor(uint32_t Color); 				   //	���û�����ɫ
void  LCD_SetBackColor(uint32_t Color);  				//	���ñ�����ɫ
void  LCD_DisplayDirection(uint8_t direction);  	//	������ʾ����

//>>>>>	��ʾ������С��
void  LCD_ShowNumMode(uint8_t mode);		// ������ʾģʽ������λ���ո������0

//>>>>>	����ͼƬ

void 	LCD_DrawImage(uint16_t x,uint16_t y,uint16_t width,uint16_t height,const uint8_t *pImage)  ;

//>>>>>	2Dͼ�λ��ƺ���

void  LCD_DrawPoint(uint16_t x,uint16_t y,uint32_t color);   								//����
uint32_t 	LCD_ReadPoint(uint16_t x,uint16_t y);												//����
void  LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);					//����
void  LCD_DrawRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height);			//������
void  LCD_DrawCircle(uint16_t x, uint16_t y, uint16_t r);									//��Բ
void  LCD_DrawEllipse(int x, int y, int r1, int r2);											//����Բ

//>>>>>	������亯��

void  LCD_FillRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height);			//������
void  LCD_FillCircle(uint16_t x, uint16_t y, uint16_t r);									//���Բ


void LCD_Test_Color(void);				// ��ɫ����
void LCD_Init_Application(void);

#ifdef __cplusplus
}
#endif

#endif
