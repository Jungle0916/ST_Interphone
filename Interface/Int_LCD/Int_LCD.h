/********************************************************
Copyright (c) 2025 Dongguan City University. All rights reserved.
FileName  : Int_LCD.h 
Author    : zheng_kai_wen 
Version   : v0.0.0.0 
Date      : 2025.09.29 
Note      : 本代码参考反客科技,但删除了原本的图像,字体显示功能
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
#define LCD_Width     	800				//	LCD的像素长度
#define LCD_Height    	480				//	LCD的像素宽度
#define LCD_MemoryAdd   0xD0000000 		//	显存的起始地址  

#define	Direction_H	0		//LCD横屏显示
#define	Direction_V	1		//LCD竖屏显示

#define  Fill_Zero  0		//填充0
#define  Fill_Space 1		//填充空格

/*---------------------------------------- 常用颜色 ------------------------------------------------------

 1. 这里为了方便用户使用，定义的是32位颜色，然后再通过代码自动转换成对应颜色格式所需要的的颜色
 2. 32位的颜色中，从高位到低位分别对应 A、R、G、B  4个颜色通道，其中A表示透明通道
 3. 最多可设置255级透明色，ff表示不透明，0表示完全透明
 4. 除非使用ARGB1555和ARGB8888等支持透明色的颜色格式，不然透明色不起作用，其中ARGB1555仅支持一位
	 透明色，即仅有透明和不透明两种状态，ARGB4444有16级透明度，ARGB8888支持255级透明度
 5. 用户可以在电脑用调色板获取24位RGB颜色，然后再补充透明通道得到32位的颜色，再将此32位颜色输入
	 LCD_SetColor()或LCD_SetBackColor()就可以显示出相应的颜色。 
 6. 使用示例：纯蓝色的RGB值为0x0000FF，如果不需要透明色，则对应的32位颜色值为 0xff0000FF
 7. 以下定义的颜色都设置为不透明，用户可根据需求自行定义对应的颜色								*/

#define 	LCD_WHITE       0xffFFFFFF		// 纯白色
#define 	LCD_BLACK       0xff000000		// 纯黑色
                           
#define 	LCD_BLUE        0xff0000FF		//	纯蓝色
#define 	LCD_GREEN       0xff00FF00    //	纯绿色
#define 	LCD_RED         0xffFF0000    //	纯红色
#define 	LCD_CYAN        0xff00FFFF    //	蓝绿色
#define 	LCD_MAGENTA     0xffFF00FF    //	紫红色
#define 	LCD_YELLOW      0xffFFFF00    //	黄色
#define 	LCD_GREY        0xff2C2C2C    //	灰色
													
#define 	LIGHT_BLUE      0xff8080FF    //	亮蓝色
#define 	LIGHT_GREEN     0xff80FF80    //	亮绿色
#define 	LIGHT_RED       0xffFF8080    //	亮红色
#define 	LIGHT_CYAN      0xff80FFFF    //	亮蓝绿色
#define 	LIGHT_MAGENTA   0xffFF80FF    //	亮紫红色
#define 	LIGHT_YELLOW    0xffFFFF80    //	亮黄色
#define 	LIGHT_GREY      0xffA3A3A3    //	亮灰色
													
#define 	DARK_BLUE       0xff000080    //	暗蓝色
#define 	DARK_GREEN      0xff008000    //	暗绿色
#define 	DARK_RED        0xff800000    //	暗红色
#define 	DARK_CYAN       0xff008080    //	暗蓝绿色
#define 	DARK_MAGENTA    0xff800080    //	暗紫红色
#define 	DARK_YELLOW     0xff808000    //	暗黄色
#define 	DARK_GREY       0xff404040    //	暗灰色

/* Private typedef -----------------------------------------------*/
/* Public variables ----------------------------------------------*/
void  LTDC_Init(void);  //LCD初始化
void  LCD_Clear(void); //清屏
void  LCD_ClearRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height);	// 局部清屏函数

void  LCD_SetLayer(uint8_t Layerx); 					//	设置层
void  LCD_SetColor(uint32_t Color); 				   //	设置画笔颜色
void  LCD_SetBackColor(uint32_t Color);  				//	设置背景颜色
void  LCD_DisplayDirection(uint8_t direction);  	//	设置显示方向

//>>>>>	显示整数或小数
void  LCD_ShowNumMode(uint8_t mode);		// 设置显示模式，多余位填充空格还是填充0

//>>>>>	绘制图片

void 	LCD_DrawImage(uint16_t x,uint16_t y,uint16_t width,uint16_t height,const uint8_t *pImage)  ;

//>>>>>	2D图形绘制函数

void  LCD_DrawPoint(uint16_t x,uint16_t y,uint32_t color);   								//画点
uint32_t 	LCD_ReadPoint(uint16_t x,uint16_t y);												//读点
void  LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);					//画线
void  LCD_DrawRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height);			//画矩形
void  LCD_DrawCircle(uint16_t x, uint16_t y, uint16_t r);									//画圆
void  LCD_DrawEllipse(int x, int y, int r1, int r2);											//画椭圆

//>>>>>	区域填充函数

void  LCD_FillRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height);			//填充矩形
void  LCD_FillCircle(uint16_t x, uint16_t y, uint16_t r);									//填充圆


void LCD_Test_Color(void);				// 颜色测试
void LCD_Init_Application(void);

#ifdef __cplusplus
}
#endif

#endif
