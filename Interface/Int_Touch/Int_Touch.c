/********************************************************
Copyright (c) 2025 Dongguan City University. All rights reserved.
FileName  : Int_Touch.c 
Author    : zheng_kai_wen 
Version   : v0.0.0.0 
Date      : 2025.09.29
Note      : 本代码参考反客科技,但是将软件IIC改成了硬件IIC2
History   : none 
*********************************************************/
/* Includes ------------------------------------------------------*/
#include "Int_Touch.h"

/* Private define ------------------------------------------------*/
/* Private typedef -----------------------------------------------*/
/* Public variables ----------------------------------------------*/
TouchStructure touchInfo;   // 触摸信息结构体，在函数 Touch_Scan() 里被调用，存储触摸数据

/* Public function prototypes ------------------------------------*/
/**
 ********************************************************************
 * name         : void GT9XX_Reset(void)
 * description  : 复位GT1158
 * Input        : none
 * Output       : none
 * Return       : none
 ********************************************************************/
void GT9XX_Reset(void)
{
    // 复位过程中需要临时改变INT引脚模式为输出
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    // 将INT引脚配置为输出
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Pin = Touch_INT_Pin;
    HAL_GPIO_Init(Touch_INT_GPIO_Port, &GPIO_InitStruct);
    
    // 初始化引脚状态
    HAL_GPIO_WritePin(Touch_INT_GPIO_Port, Touch_INT_Pin, GPIO_PIN_RESET);  // INT输出低电平
    HAL_GPIO_WritePin(Touch_RST_GPIO_Port, Touch_RST_Pin, GPIO_PIN_SET);    // RST输出高电平
    HAL_Delay(10);
    
    // 开始执行复位
    // INT引脚保持低电平不变，将器件地址设置为0x28/0x29
    HAL_GPIO_WritePin(Touch_RST_GPIO_Port, Touch_RST_Pin, GPIO_PIN_RESET); // 拉低复位引脚，此时芯片执行复位
    HAL_Delay(25);
    HAL_GPIO_WritePin(Touch_RST_GPIO_Port, Touch_RST_Pin, GPIO_PIN_SET);   // 拉高复位引脚，复位结束
    HAL_Delay(45);
    
    // 复位完成后恢复INT引脚为输入模式
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Pin = Touch_INT_Pin;
    HAL_GPIO_Init(Touch_INT_GPIO_Port, &GPIO_InitStruct);
    
    HAL_Delay(35);
}

/**
 ********************************************************************
 * name         : uint8_t Touch_Init(void)
 * description  : 触摸IC初始化，并读取相应信息
 * Input        : none
 * Output       : none
 * Return       : SUCCESS - 初始化成功
 *                ERROR   - 错误，未检测到触摸屏
 ********************************************************************/
uint8_t Touch_Init(void)
{
    uint8_t GT9XX_Info[11]; // 触摸屏IC信息
    HAL_StatusTypeDef status;
    
    GT9XX_Reset(); // 复位IC
    HAL_Delay(100);
    
    // 读触摸屏IC信息
    status = HAL_I2C_Mem_Read(&hi2c2, GT9XX_IIC_RADDR, GT9XX_ID_ADDR, I2C_MEMADD_SIZE_16BIT, GT9XX_Info, 11, 100);
    if(status == HAL_OK)
    {
        printf("读取到的芯片ID数据: ");
        for(int i = 0; i < 11; i++)
        {
            printf("0x%02X ", GT9XX_Info[i]);
        }
        printf("\r\n");
        
        // 打印芯片信息
        printf("Touch ID: %.4s \r\n", GT9XX_Info);  // GT1158
        printf("固件版本: 0X%.4x\r\n", (GT9XX_Info[5]<<8) + GT9XX_Info[4]);
        printf("触摸分辨率: %d * %d\r\n", (GT9XX_Info[7]<<8) + GT9XX_Info[6], (GT9XX_Info[9]<<8) + GT9XX_Info[8]);
        return SUCCESS;
    }
    else
    {
        printf("读取芯片ID信息失败，错误代码: %d\r\n", status);
        if(status == HAL_ERROR) printf("HAL_ERROR\r\n");
        if(status == HAL_BUSY) printf("HAL_BUSY\r\n");
        if(status == HAL_TIMEOUT) printf("HAL_TIMEOUT\r\n");
    }
    
    // 未检测到触摸IC
    printf("未检测到触摸IC\r\n");
    return ERROR;
}

/**
 ********************************************************************
 * name         : void Touch_Scan(void)
 * description  : 触摸扫描
 * Input        : none
 * Output       : 更新touchInfo结构体中的触摸数据
 * Return       : none
 ********************************************************************/
void Touch_Scan(void)
{
    uint8_t touchData[2 + 8 * TOUCH_MAX]; // 用于存储触摸数据
    uint8_t i = 0;
    HAL_StatusTypeDef status;
    
    // 读数据
    status = HAL_I2C_Mem_Read(&hi2c2, GT9XX_IIC_RADDR, GT9XX_READ_ADDR, I2C_MEMADD_SIZE_16BIT, touchData, 2 + 8 * TOUCH_MAX, 100);
    if(status == HAL_OK)
    {
        // 清除触摸芯片的寄存器标志位
        uint8_t clearFlag = 0;
        HAL_I2C_Mem_Write(&hi2c2, GT9XX_IIC_WADDR, GT9XX_READ_ADDR, I2C_MEMADD_SIZE_16BIT, &clearFlag, 1, 100);
        
        touchInfo.num = touchData[0] & 0x0f; // 取当前的触摸点数
        
        if((touchInfo.num >= 1) && (touchInfo.num <= 5)) // 当触摸数在 1-5 之间时
        {
            for(i = 0; i < touchInfo.num; i++) // 取相应的触摸坐标
            {
                touchInfo.y[i] = (touchData[5 + 8 * i] << 8) | touchData[4 + 8 * i]; // 获取Y坐标
                touchInfo.x[i] = (touchData[3 + 8 * i] << 8) | touchData[2 + 8 * i]; // 获取X坐标
            }
            touchInfo.flag = 1; // 触摸标志位置1，代表有触摸动作发生
        }
        else
        {
            touchInfo.flag = 0; // 触摸标志位置0，无触摸动作
        }
    }
    else
    {
        // I2C读取失败时清零触摸信息
        touchInfo.flag = 0;
        touchInfo.num = 0;
    }
}
