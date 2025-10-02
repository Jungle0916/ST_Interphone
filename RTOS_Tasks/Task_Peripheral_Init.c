/********************************************************
Copyright (c) 2025 Dongguan City University. All rights reserved.
FileName  : Task_Peripheral_Init.c 
Author    : zheng_kai_wen 
Version   : v0.0.0.0 
Date      : 2025.09.28 
Note      : 用于初始化外设
History   : none 
*********************************************************/
/* Includes ------------------------------------------------------*/
#include "Task_Peripheral_Init.h"
#include "ff.h"
#include "bsp_driver_sd.h"
#include "sd_diskio.h"
#include "Com_Debug.h"

/* Private define ------------------------------------------------*/
/* Private typedef -----------------------------------------------*/
/* Public variables ----------------------------------------------*/

/* Public function prototypes ------------------------------------*/
/**
 ********************************************************************
 * name         : Task_Peripheral_Init(void *argument)
 * description  : 初始化SD卡
 * Input        : none
 * Output       : none
 * Return       : none
 ********************************************************************/
void Task_Peripheral_Init(void *argument)
{
    // SD卡测试
    debug_printf("开始SD卡初始化...\r\n");

    // 检查SD卡是否被检测到
    if(BSP_SD_IsDetected() != SD_PRESENT)
    {
        debug_printf("未检测到SD卡！请检查连接。\r\n");
    }
    else
    {
        debug_printf("检测到SD卡，正在初始化...\r\n");
        
        // 初始化SD卡
        uint8_t sd_result = BSP_SD_Init();
        debug_printf("BSP_SD_Init返回值: %d (MSD_OK=%d)\r\n", sd_result, MSD_OK);
        if(sd_result == MSD_OK)
        {
            debug_printf("SD卡初始化成功！\r\n");
            
            // 打印SD卡信息
            HAL_SD_CardInfoTypeDef card_info;
            BSP_SD_GetCardInfo(&card_info);  // 使用BSP函数而不是直接访问hsd
            debug_printf("SD卡类型: %d\r\n", card_info.CardType);
            debug_printf("SD卡版本: %d.%d\r\n", card_info.CardVersion >> 4, card_info.CardVersion & 0x0F);
            debug_printf("SD卡大小: %lu MB\r\n", (uint32_t)(card_info.BlockNbr * card_info.BlockSize) / (1024 * 1024));
            
            // 挂载文件系统
            FRESULT res = f_mount(&SDFatFS, (TCHAR const*)SDPath, 1);
            debug_printf("f_mount返回值: %d (FR_OK=%d)\r\n", res, FR_OK);
            if(res == FR_OK)
            {
                debug_printf("文件系统挂载成功！\r\n");
                
                // 显示SD卡信息
                FATFS *fs;
                DWORD free_clust, tot_sect, free_sect;
                
                // 获取空闲扇区数
                res = f_getfree(SDPath, &free_clust, &fs);
                if(res == FR_OK)
                {
                    tot_sect = (fs->n_fatent - 2) * fs->csize;
                    free_sect = free_clust * fs->csize;
                    
                    debug_printf("SD卡总容量: %lu KB\r\n", tot_sect / 2);
                    debug_printf("SD卡可用空间: %lu KB\r\n", free_sect / 2);
                    
                    // 在这里可以添加您的应用代码
                    // 例如创建文件、写入数据等
                }
            }
            else
            {
                debug_printf("文件系统挂载失败！错误码: %d\r\n", res);
                // 尝试格式化
                if(res == FR_NO_FILESYSTEM)
                {
                    debug_printf("未找到文件系统，尝试格式化...\r\n");
                    // 使用较小的缓冲区，避免栈溢出
                    BYTE work[512];
                    res = f_mkfs(SDPath, FM_FAT32, 0, work, sizeof(work));
                    if(res == FR_OK)
                    {
                        debug_printf("格式化成功！\r\n");
                        // 重新挂载
                        res = f_mount(&SDFatFS, (TCHAR const*)SDPath, 1);
                        if(res == FR_OK)
                        {
                            debug_printf("文件系统重新挂载成功！\r\n");
                            
                            // 显示SD卡信息
                            FATFS *fs;
                            DWORD free_clust, tot_sect, free_sect;
                            
                            // 获取空闲扇区数
                            res = f_getfree(SDPath, &free_clust, &fs);
                            if(res == FR_OK)
                            {
                                tot_sect = (fs->n_fatent - 2) * fs->csize;
                                free_sect = free_clust * fs->csize;
                                
                                debug_printf("SD卡总容量: %lu KB\r\n", tot_sect / 2);
                                debug_printf("SD卡可用空间: %lu KB\r\n", free_sect / 2);
                            }
                        }
                    }
                    else
                    {
                        debug_printf("格式化失败！错误码: %d\r\n", res);
                    }
                }
            }
        }
        else
        {
            debug_printf("SD卡初始化失败！错误码: %d\r\n", sd_result);
            // 根据错误码提供更多信息
            switch(sd_result) {
                case MSD_ERROR:
                    debug_printf("SD卡初始化发生错误\r\n");
                    break;
                case MSD_OK:
                    debug_printf("SD卡初始化成功\r\n");
                    break;
                default:
                    debug_printf("未知错误代码\r\n");
                    break;
            }
        }
    }
    
    // 任务完成，可以删除自身或者进入空循环
    vTaskDelete(NULL);
}
