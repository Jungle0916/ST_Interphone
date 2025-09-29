/********************************************************
Copyright (c) 2025 Year, Dongguan City University., Ltd. All rights reserved.
FileName  : Int_LED.h 
Author    : zheng_kai_wen 
Version   : v0.0.0.0 
Date      : 2025.09.26 
Note      : none
History   : none 
*********************************************************/
#ifndef __INT_LED_H
#define	__INT_LED_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------*/
#include "main.h"
/* Public define ------------------------------------------------*/
#define LED1_PIN    LED1_Pin
#define LED1_PORT   LED1_GPIO_Port
#define LED2_PIN    LED2_Pin
#define LED2_PORT   LED2_GPIO_Port
/* Private typedef -----------------------------------------------*/

/* Public variables ----------------------------------------------*/

/* Public function prototypes ------------------------------------*/
void LED_On(uint16_t led_pin);      //LED¿ª
void LED_Off(uint16_t led_pin);     //LED¹Ø
void LED_Toggle(uint16_t led_pin);  //LED·­×ª

#ifdef __cplusplus
}
#endif

#endif
