/********************************************************
Copyright (c) 2025 Year, Dongguan City University., Ltd. All rights reserved.
FileName  : Int_LED.c 
Author    : zheng_kai_wen 
Version   : v0.0.0.0 
Date      : 2025.09.26 
Note      : none
History   : none 
*********************************************************/
/* Includes ------------------------------------------------------*/
#include "Int_LED.h" 
/* Private define ------------------------------------------------*/
/* Private typedef -----------------------------------------------*/
/* Public variables ----------------------------------------------*/

/* Public function prototypes ------------------------------------*/
/**
 ********************************************************************
 * name         : void LED_On(uint16_t led_pin)
 * description  : 打开指定的LED
 * Input        : led_pin: LED引脚 (LED1_PIN 或 LED2_PIN)
 * Output       : LED引脚状态设置为低电平
 * Return       : none
 ********************************************************************/
void LED_On(uint16_t led_pin) 
{
    if (led_pin == LED1_PIN) 
    {
        HAL_GPIO_WritePin(LED1_PORT, LED1_PIN, GPIO_PIN_RESET);
    } 
    else if (led_pin == LED2_PIN) 
    {
        HAL_GPIO_WritePin(LED2_PORT, LED2_PIN, GPIO_PIN_RESET);
    }
}

/**
 ********************************************************************
 * name         : void LED_Off(uint16_t led_pin)
 * description  : 关闭指定的LED
 * Input        : led_pin: LED引脚 (LED1_PIN 或 LED2_PIN)
 * Output       : LED引脚状态设置为高电平
 * Return       : none
 ********************************************************************/
void LED_Off(uint16_t led_pin) 
{
    if (led_pin == LED1_PIN) 
    {
        HAL_GPIO_WritePin(LED1_PORT, LED1_PIN, GPIO_PIN_SET);
    } 
    else if (led_pin == LED2_PIN) 
    {
        HAL_GPIO_WritePin(LED2_PORT, LED2_PIN, GPIO_PIN_SET);
    }
}

/**
 ********************************************************************
 * name         : void LED_Toggle(uint16_t led_pin)
 * description  : 切换指定LED的状态
 * Input        : led_pin: LED引脚 (LED1_PIN 或 LED2_PIN)
 * Output       : LED引脚状态切换
 * Return       : none
 ********************************************************************/
void LED_Toggle(uint16_t led_pin) 
{
    if (led_pin == LED1_PIN) 
    {
        HAL_GPIO_TogglePin(LED1_PORT, LED1_PIN);
    } 
    else if (led_pin == LED2_PIN) 
    {
        HAL_GPIO_TogglePin(LED2_PORT, LED2_PIN);
    }
}
