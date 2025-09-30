/********************************************************
Copyright (c) 2025 Dongguan City University. All rights reserved.
FileName  : Int_W25Q64.c 
Author    : zheng_kai_wen 
Version   : v0.0.0.0 
Date      : 2025.09.26 
Note      : none
History   : none 
*********************************************************/
/* Includes ------------------------------------------------------*/
#include "Int_W25Q64.h"
#include <stdlib.h>
/* Private define ------------------------------------------------*/
/* Private typedef -----------------------------------------------*/
/* Public variables ----------------------------------------------*/

/* Public function prototypes ------------------------------------*/
/**
 ********************************************************************
 * name         : static void W25Q64_Write_Enable(void)
 * description  : W25Q64写使能
 * Input        : none
 * Output       : none
 * Return       : none
 ********************************************************************/
static void W25Q64_Write_Enable(void)
{
    W25Q64_CS_LOW();
    W25Q64_Send_Byte(W25X_WriteEnable);
    W25Q64_CS_HIGH();
}

/**
 ********************************************************************
 * name         : static void W25Q64_Wait_Busy(void)
 * description  : 等待空闲
 * Input        : none
 * Output       : none
 * Return       : none
 ********************************************************************/
static void W25Q64_Wait_Busy(void)
{
    uint8_t status = 0;
    do {
        W25Q64_CS_LOW();
        W25Q64_Send_Byte(W25X_ReadStatusReg);
        status = W25Q64_Send_Byte(DUMMY_BYTE);
        W25Q64_CS_HIGH();
    } while((status & WIP_Flag) == WIP_Flag);
}

/**
 ********************************************************************
 * name         : static uint8_t W25Q64_Send_Byte(uint8_t byte)
 * description  : 发送一个字节并通过SPI接口接收一个字节
 * Input        : byte: 要发送的字节
 * Output       : none
 * Return       : 接收到的字节
 ********************************************************************/
static uint8_t W25Q64_Send_Byte(uint8_t byte)
{
    uint8_t receivedByte = 0;
    
    // 等待发送寄存器为空
    while(HAL_SPI_GetState(&hspi3) != HAL_SPI_STATE_READY);
    
    // 发送并接收数据
    HAL_SPI_TransmitReceive(&hspi3, &byte, &receivedByte, 1, 100);
    
    return receivedByte;
}

/**
 ********************************************************************
 * name         : uint32_t W25Q64_ReadID(void)
 * description  : 读取W25Q64芯片ID
 * Input        : none
 * Output       : none
 * Return       : 芯片ID
 ********************************************************************/
uint32_t W25Q64_ReadID(void)
{
    uint32_t Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

    W25Q64_CS_LOW();
    
    W25Q64_Send_Byte(W25X_ManufactDeviceID);
    Temp0 = W25Q64_Send_Byte(DUMMY_BYTE);
    Temp1 = W25Q64_Send_Byte(DUMMY_BYTE);
    Temp2 = W25Q64_Send_Byte(DUMMY_BYTE);
    
    W25Q64_CS_HIGH();
    
    Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;
    return Temp;
}

/**
 ********************************************************************
 * name         : void W25Q64_Read(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t Size)
 * description  : 读取SPI FLASH
 * Input        : pBuffer: 读取数据缓冲区
 *                ReadAddr: 读取地址
 *                Size: 读取数据大小
 * Output       : none
 * Return       : none
 ********************************************************************/
void W25Q64_Read(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t Size)
{
    W25Q64_CS_LOW();
    
    W25Q64_Send_Byte(W25X_ReadData);
    W25Q64_Send_Byte((ReadAddr & 0xFF0000) >> 16);
    W25Q64_Send_Byte((ReadAddr & 0xFF00) >> 8);
    W25Q64_Send_Byte(ReadAddr & 0xFF);
    
    while(Size--) {
        *pBuffer = W25Q64_Send_Byte(DUMMY_BYTE);
        pBuffer++;
    }
    
    W25Q64_CS_HIGH();
}

/**
 ********************************************************************
 * name         : void W25Q64_Erase_Chip(void)
 * description  : 擦除整个芯片
 * Input        : none
 * Output       : none
 * Return       : none
 ********************************************************************/
void W25Q64_Erase_Chip(void)
{
    W25Q64_Write_Enable();
    W25Q64_Wait_Busy();
    
    W25Q64_CS_LOW();
    W25Q64_Send_Byte(W25X_ChipErase);
    W25Q64_CS_HIGH();
    
    W25Q64_Wait_Busy();
}

/**
 ********************************************************************
 * name         : void W25Q64_Erase_Sector(uint32_t Dst_Addr)
 * description  : 擦除一个扇区
 * Input        : Dst_Addr: 扇区地址
 * Output       : none
 * Return       : none
 ********************************************************************/
void W25Q64_Erase_Sector(uint32_t Dst_Addr)
{
    Dst_Addr *= W25Q64_SECTOR_SIZE;
    
    W25Q64_Write_Enable();
    W25Q64_Wait_Busy();
    
    W25Q64_CS_LOW();
    W25Q64_Send_Byte(W25X_SectorErase);
    W25Q64_Send_Byte((Dst_Addr & 0xFF0000) >> 16);
    W25Q64_Send_Byte((Dst_Addr & 0xFF00) >> 8);
    W25Q64_Send_Byte(Dst_Addr & 0xFF);
    W25Q64_CS_HIGH();
    
    W25Q64_Wait_Busy();
}

/**
 ********************************************************************
 * name         : void W25Q64_Write_Page(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t Size)
 * description  : 页写入数据
 * Input        : pBuffer: 数据缓冲区
 *                WriteAddr: 写入地址
 *                Size: 数据大小
 * Output       : none
 * Return       : none
 ********************************************************************/
void W25Q64_Write_Page(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t Size)
{
    W25Q64_Write_Enable();
    
    W25Q64_CS_LOW();
    W25Q64_Send_Byte(W25X_PageProgram);
    W25Q64_Send_Byte((WriteAddr & 0xFF0000) >> 16);
    W25Q64_Send_Byte((WriteAddr & 0xFF00) >> 8);
    W25Q64_Send_Byte(WriteAddr & 0xFF);
    
    while(Size--) {
        W25Q64_Send_Byte(*pBuffer);
        pBuffer++;
    }
    
    W25Q64_CS_HIGH();
    
    W25Q64_Wait_Busy();
}

/**
 ********************************************************************
 * name         : void W25Q64_Write_NoCheck(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t Size)
 * description  : 无校验写入数据
 * Input        : pBuffer: 数据缓冲区
 *                WriteAddr: 写入地址
 *                Size: 数据大小
 * Output       : none
 * Return       : none
 ********************************************************************/
void W25Q64_Write_NoCheck(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t Size)
{
    uint16_t pageremain;
    pageremain = W25Q64_PAGE_SIZE - (WriteAddr % W25Q64_PAGE_SIZE);
    
    if(Size <= pageremain)
        pageremain = Size;
    
    while(1)
    {
        W25Q64_Write_Page(pBuffer, WriteAddr, pageremain);
        
        if(Size == pageremain)
            break;
        else 
        {
            pBuffer += pageremain;
            WriteAddr += pageremain;
            Size -= pageremain;
            
            if(Size > W25Q64_PAGE_SIZE)
                pageremain = W25Q64_PAGE_SIZE;
            else 
                pageremain = Size;
        }
    }
}

/**
 ********************************************************************
 * name         : void W25Q64_Write(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t Size)
 * description  : 写入数据
 * Input        : pBuffer: 数据缓冲区
 *                WriteAddr: 写入地址
 *                Size: 数据大小
 * Output       : none
 * Return       : none
 ********************************************************************/
void W25Q64_Write(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t Size)
{
    uint32_t secpos;
    uint16_t secoff;
    uint16_t secremain;
    uint16_t i;
    
    uint8_t *W25Q64_BUF;
    W25Q64_BUF = (uint8_t*)malloc(W25Q64_SECTOR_SIZE);
    
    if(W25Q64_BUF == NULL)
    {
        // 如果分配内存失败，则直接写入（不进行擦除检查）
        W25Q64_Write_NoCheck(pBuffer, WriteAddr, Size);
        return;
    }
    
    secpos = WriteAddr / W25Q64_SECTOR_SIZE;
    secoff = WriteAddr % W25Q64_SECTOR_SIZE;
    secremain = W25Q64_SECTOR_SIZE - secoff;
    
    if(Size <= secremain)
        secremain = Size;
    
    while(1)
    {
        W25Q64_Read(W25Q64_BUF, secpos * W25Q64_SECTOR_SIZE, W25Q64_SECTOR_SIZE);
        
        for(i = 0; i < secremain; i++)
        {
            if(W25Q64_BUF[secoff + i] != 0xFF)
                break;
        }
        
        if(i < secremain)
        {
            // 需要擦除
            W25Q64_Erase_Sector(secpos);
            for(i = 0; i < secremain; i++)
            {
                W25Q64_BUF[i + secoff] = pBuffer[i];
            }
            W25Q64_Write_NoCheck(W25Q64_BUF, secpos * W25Q64_SECTOR_SIZE, W25Q64_SECTOR_SIZE);
        }
        else
        {
            // 无需擦除
            W25Q64_Write_NoCheck(pBuffer, WriteAddr, secremain);
        }
        
        if(Size == secremain)
            break;
        else
        {
            secpos++;
            secoff = 0;
            
            pBuffer += secremain;
            WriteAddr += secremain;
            Size -= secremain;
            
            if(Size > W25Q64_SECTOR_SIZE)
                secremain = W25Q64_SECTOR_SIZE;
            else
                secremain = Size;
        }
    }
    
    free(W25Q64_BUF);
}

/**
 ********************************************************************
 * name         : void W25Q64_PowerDown(void)
 * description  : 进入掉电模式
 * Input        : none
 * Output       : none
 * Return       : none
 ********************************************************************/
void W25Q64_PowerDown(void)
{
    W25Q64_CS_LOW();
    W25Q64_Send_Byte(W25X_PowerDown);
    W25Q64_CS_HIGH();
    
    HAL_Delay(1); // 等待芯片进入掉电模式
}

/**
 ********************************************************************
 * name         : void W25Q64_WAKEUP(void)
 * description  : 唤醒
 * Input        : none
 * Output       : none
 * Return       : none
 ********************************************************************/
void W25Q64_WAKEUP(void)
{
    W25Q64_CS_LOW();
    W25Q64_Send_Byte(W25X_ReleasePowerDown);
    W25Q64_CS_HIGH();
    
    HAL_Delay(1); // 等待芯片唤醒
}

/**
 ********************************************************************
 * name         : void W25Q64_Init(void)
 * description  : 初始化W25Q64
 * Input        : none
 * Output       : none
 * Return       : none
 ********************************************************************/
void W25Q64_Init(void)
{
    // 初始化CS引脚为高电平（非常重要，参考代码中特别提到）
    W25Q64_CS_HIGH();
    
    // 延时确保Flash芯片稳定
    HAL_Delay(10);
    
    // 检查芯片ID
    uint32_t flash_id = W25Q64_ReadID();
    if(flash_id == 0xEF4017)  // W25Q64的典型ID
    {
        // 初始化成功
    }
}
