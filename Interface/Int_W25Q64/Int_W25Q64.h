/********************************************************
Copyright (c) 2025 Dongguan City University. All rights reserved.
FileName  : Int_W25Q64.h 
Author    : zheng_kai_wen 
Version   : v0.0.0.0 
Date      : 2025.09.26 
Note      : none
History   : none 
*********************************************************/
#ifndef __INT_W25Q64_H
#define	__INT_W25Q64_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------*/
#include "main.h"
/* Public define ------------------------------------------------*/
#define W25Q64_CS_LOW()     HAL_GPIO_WritePin(SPI_Flash_CS_GPIO_Port, SPI_Flash_CS_Pin, GPIO_PIN_RESET)
#define W25Q64_CS_HIGH()    HAL_GPIO_WritePin(SPI_Flash_CS_GPIO_Port, SPI_Flash_CS_Pin, GPIO_PIN_SET)

/* W25Q64指令表 */
#define W25X_WriteEnable		  0x06 
#define W25X_WriteDisable		  0x04 
#define W25X_ReadStatusReg		0x05 
#define W25X_WriteStatusReg		0x01 
#define W25X_ReadData			    0x03 
#define W25X_FastReadData		  0x0B 
#define W25X_PageProgram		  0x02 
#define W25X_BlockErase			  0xD8 
#define W25X_SectorErase		  0x20 
#define W25X_ChipErase			  0xC7 
#define W25X_PowerDown			  0xB9 
#define W25X_ReleasePowerDown	0xAB 
#define W25X_DeviceID			    0xAB 
#define W25X_ManufactDeviceID	0x90 
#define W25X_JedecDeviceID		0x9F 

/* W25Q64参数 */
#define W25Q64_PAGE_SIZE      256      // 页大小为256字节
#define W25Q64_SECTOR_SIZE    4096     // 扇区大小为4KB
#define W25Q64_BLOCK_SIZE     65536    // 块大小为64KB
#define W25Q64_CAPACITY       0x800000 // 总容量为8MB

/* 状态寄存器位定义 */
#define WIP_Flag              0x01     // 写入忙标志位
#define DUMMY_BYTE            0xFF     // 虚拟字节

/* Private typedef -----------------------------------------------*/

/* Public variables ----------------------------------------------*/
extern SPI_HandleTypeDef hspi3;
/* Public function prototypes ------------------------------------*/
void W25Q64_Init(void);
uint32_t W25Q64_ReadID(void);
void W25Q64_Read(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t Size);
void W25Q64_Write(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t Size);
void W25Q64_Erase_Chip(void);
void W25Q64_Erase_Sector(uint32_t Dst_Addr);
void W25Q64_Write_Page(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t Size);
void W25Q64_Write_NoCheck(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t Size);
void W25Q64_PowerDown(void);
void W25Q64_WAKEUP(void);

static void W25Q64_Write_Enable(void);
static void W25Q64_Wait_Busy(void);
static uint8_t W25Q64_Send_Byte(uint8_t byte);

#ifdef __cplusplus
}
#endif

#endif
