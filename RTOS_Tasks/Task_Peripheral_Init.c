/********************************************************
Copyright (c) 2025 Dongguan City University. All rights reserved.
FileName  : Task_Peripheral_Init.c 
Author    : zheng_kai_wen 
Version   : v0.0.0.0 
Date      : 2025.09.28 
Note      : ���ڳ�ʼ������
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
 * description  : ��ʼ��SD��
 * Input        : none
 * Output       : none
 * Return       : none
 ********************************************************************/
void Task_Peripheral_Init(void *argument)
{
    // SD������
    debug_printf("��ʼSD����ʼ��...\r\n");

    // ���SD���Ƿ񱻼�⵽
    if(BSP_SD_IsDetected() != SD_PRESENT)
    {
        debug_printf("δ��⵽SD�����������ӡ�\r\n");
    }
    else
    {
        debug_printf("��⵽SD�������ڳ�ʼ��...\r\n");
        
        // ��ʼ��SD��
        uint8_t sd_result = BSP_SD_Init();
        debug_printf("BSP_SD_Init����ֵ: %d (MSD_OK=%d)\r\n", sd_result, MSD_OK);
        if(sd_result == MSD_OK)
        {
            debug_printf("SD����ʼ���ɹ���\r\n");
            
            // ��ӡSD����Ϣ
            HAL_SD_CardInfoTypeDef card_info;
            BSP_SD_GetCardInfo(&card_info);  // ʹ��BSP����������ֱ�ӷ���hsd
            debug_printf("SD������: %d\r\n", card_info.CardType);
            debug_printf("SD���汾: %d.%d\r\n", card_info.CardVersion >> 4, card_info.CardVersion & 0x0F);
            debug_printf("SD����С: %lu MB\r\n", (uint32_t)(card_info.BlockNbr * card_info.BlockSize) / (1024 * 1024));
            
            // �����ļ�ϵͳ
            FRESULT res = f_mount(&SDFatFS, (TCHAR const*)SDPath, 1);
            debug_printf("f_mount����ֵ: %d (FR_OK=%d)\r\n", res, FR_OK);
            if(res == FR_OK)
            {
                debug_printf("�ļ�ϵͳ���سɹ���\r\n");
                
                // ��ʾSD����Ϣ
                FATFS *fs;
                DWORD free_clust, tot_sect, free_sect;
                
                // ��ȡ����������
                res = f_getfree(SDPath, &free_clust, &fs);
                if(res == FR_OK)
                {
                    tot_sect = (fs->n_fatent - 2) * fs->csize;
                    free_sect = free_clust * fs->csize;
                    
                    debug_printf("SD��������: %lu KB\r\n", tot_sect / 2);
                    debug_printf("SD�����ÿռ�: %lu KB\r\n", free_sect / 2);
                    
                    // ����������������Ӧ�ô���
                    // ���紴���ļ���д�����ݵ�
                }
            }
            else
            {
                debug_printf("�ļ�ϵͳ����ʧ�ܣ�������: %d\r\n", res);
                // ���Ը�ʽ��
                if(res == FR_NO_FILESYSTEM)
                {
                    debug_printf("δ�ҵ��ļ�ϵͳ�����Ը�ʽ��...\r\n");
                    // ʹ�ý�С�Ļ�����������ջ���
                    BYTE work[512];
                    res = f_mkfs(SDPath, FM_FAT32, 0, work, sizeof(work));
                    if(res == FR_OK)
                    {
                        debug_printf("��ʽ���ɹ���\r\n");
                        // ���¹���
                        res = f_mount(&SDFatFS, (TCHAR const*)SDPath, 1);
                        if(res == FR_OK)
                        {
                            debug_printf("�ļ�ϵͳ���¹��سɹ���\r\n");
                            
                            // ��ʾSD����Ϣ
                            FATFS *fs;
                            DWORD free_clust, tot_sect, free_sect;
                            
                            // ��ȡ����������
                            res = f_getfree(SDPath, &free_clust, &fs);
                            if(res == FR_OK)
                            {
                                tot_sect = (fs->n_fatent - 2) * fs->csize;
                                free_sect = free_clust * fs->csize;
                                
                                debug_printf("SD��������: %lu KB\r\n", tot_sect / 2);
                                debug_printf("SD�����ÿռ�: %lu KB\r\n", free_sect / 2);
                            }
                        }
                    }
                    else
                    {
                        debug_printf("��ʽ��ʧ�ܣ�������: %d\r\n", res);
                    }
                }
            }
        }
        else
        {
            debug_printf("SD����ʼ��ʧ�ܣ�������: %d\r\n", sd_result);
            // ���ݴ������ṩ������Ϣ
            switch(sd_result) {
                case MSD_ERROR:
                    debug_printf("SD����ʼ����������\r\n");
                    break;
                case MSD_OK:
                    debug_printf("SD����ʼ���ɹ�\r\n");
                    break;
                default:
                    debug_printf("δ֪�������\r\n");
                    break;
            }
        }
    }
    
    // ������ɣ�����ɾ��������߽����ѭ��
    vTaskDelete(NULL);
}
