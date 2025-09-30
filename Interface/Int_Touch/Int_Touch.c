/********************************************************
Copyright (c) 2025 Dongguan City University. All rights reserved.
FileName  : Int_Touch.c 
Author    : zheng_kai_wen 
Version   : v0.0.0.0 
Date      : 2025.09.29
Note      : ������ο����ͿƼ�,���ǽ����IIC�ĳ���Ӳ��IIC2
History   : none 
*********************************************************/
/* Includes ------------------------------------------------------*/
#include "Int_Touch.h"

/* Private define ------------------------------------------------*/
/* Private typedef -----------------------------------------------*/
/* Public variables ----------------------------------------------*/
TouchStructure touchInfo;   // ������Ϣ�ṹ�壬�ں��� Touch_Scan() �ﱻ���ã��洢��������

/* Public function prototypes ------------------------------------*/
/**
 ********************************************************************
 * name         : void GT9XX_Reset(void)
 * description  : ��λGT1158
 * Input        : none
 * Output       : none
 * Return       : none
 ********************************************************************/
void GT9XX_Reset(void)
{
    // ��λ��������Ҫ��ʱ�ı�INT����ģʽΪ���
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    // ��INT��������Ϊ���
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Pin = Touch_INT_Pin;
    HAL_GPIO_Init(Touch_INT_GPIO_Port, &GPIO_InitStruct);
    
    // ��ʼ������״̬
    HAL_GPIO_WritePin(Touch_INT_GPIO_Port, Touch_INT_Pin, GPIO_PIN_RESET);  // INT����͵�ƽ
    HAL_GPIO_WritePin(Touch_RST_GPIO_Port, Touch_RST_Pin, GPIO_PIN_SET);    // RST����ߵ�ƽ
    HAL_Delay(10);
    
    // ��ʼִ�и�λ
    // INT���ű��ֵ͵�ƽ���䣬��������ַ����Ϊ0x28/0x29
    HAL_GPIO_WritePin(Touch_RST_GPIO_Port, Touch_RST_Pin, GPIO_PIN_RESET); // ���͸�λ���ţ���ʱоƬִ�и�λ
    HAL_Delay(25);
    HAL_GPIO_WritePin(Touch_RST_GPIO_Port, Touch_RST_Pin, GPIO_PIN_SET);   // ���߸�λ���ţ���λ����
    HAL_Delay(45);
    
    // ��λ��ɺ�ָ�INT����Ϊ����ģʽ
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Pin = Touch_INT_Pin;
    HAL_GPIO_Init(Touch_INT_GPIO_Port, &GPIO_InitStruct);
    
    HAL_Delay(35);
}

/**
 ********************************************************************
 * name         : uint8_t Touch_Init(void)
 * description  : ����IC��ʼ��������ȡ��Ӧ��Ϣ
 * Input        : none
 * Output       : none
 * Return       : SUCCESS - ��ʼ���ɹ�
 *                ERROR   - ����δ��⵽������
 ********************************************************************/
uint8_t Touch_Init(void)
{
    uint8_t GT9XX_Info[11]; // ������IC��Ϣ
    HAL_StatusTypeDef status;
    
    GT9XX_Reset(); // ��λIC
    HAL_Delay(100);
    
    // ��������IC��Ϣ
    status = HAL_I2C_Mem_Read(&hi2c2, GT9XX_IIC_RADDR, GT9XX_ID_ADDR, I2C_MEMADD_SIZE_16BIT, GT9XX_Info, 11, 100);
    if(status == HAL_OK)
    {
        printf("��ȡ����оƬID����: ");
        for(int i = 0; i < 11; i++)
        {
            printf("0x%02X ", GT9XX_Info[i]);
        }
        printf("\r\n");
        
        // ��ӡоƬ��Ϣ
        printf("Touch ID: %.4s \r\n", GT9XX_Info);  // GT1158
        printf("�̼��汾: 0X%.4x\r\n", (GT9XX_Info[5]<<8) + GT9XX_Info[4]);
        printf("�����ֱ���: %d * %d\r\n", (GT9XX_Info[7]<<8) + GT9XX_Info[6], (GT9XX_Info[9]<<8) + GT9XX_Info[8]);
        return SUCCESS;
    }
    else
    {
        printf("��ȡоƬID��Ϣʧ�ܣ��������: %d\r\n", status);
        if(status == HAL_ERROR) printf("HAL_ERROR\r\n");
        if(status == HAL_BUSY) printf("HAL_BUSY\r\n");
        if(status == HAL_TIMEOUT) printf("HAL_TIMEOUT\r\n");
    }
    
    // δ��⵽����IC
    printf("δ��⵽����IC\r\n");
    return ERROR;
}

/**
 ********************************************************************
 * name         : void Touch_Scan(void)
 * description  : ����ɨ��
 * Input        : none
 * Output       : ����touchInfo�ṹ���еĴ�������
 * Return       : none
 ********************************************************************/
void Touch_Scan(void)
{
    uint8_t touchData[2 + 8 * TOUCH_MAX]; // ���ڴ洢��������
    uint8_t i = 0;
    HAL_StatusTypeDef status;
    
    // ������
    status = HAL_I2C_Mem_Read(&hi2c2, GT9XX_IIC_RADDR, GT9XX_READ_ADDR, I2C_MEMADD_SIZE_16BIT, touchData, 2 + 8 * TOUCH_MAX, 100);
    if(status == HAL_OK)
    {
        // �������оƬ�ļĴ�����־λ
        uint8_t clearFlag = 0;
        HAL_I2C_Mem_Write(&hi2c2, GT9XX_IIC_WADDR, GT9XX_READ_ADDR, I2C_MEMADD_SIZE_16BIT, &clearFlag, 1, 100);
        
        touchInfo.num = touchData[0] & 0x0f; // ȡ��ǰ�Ĵ�������
        
        if((touchInfo.num >= 1) && (touchInfo.num <= 5)) // ���������� 1-5 ֮��ʱ
        {
            for(i = 0; i < touchInfo.num; i++) // ȡ��Ӧ�Ĵ�������
            {
                touchInfo.y[i] = (touchData[5 + 8 * i] << 8) | touchData[4 + 8 * i]; // ��ȡY����
                touchInfo.x[i] = (touchData[3 + 8 * i] << 8) | touchData[2 + 8 * i]; // ��ȡX����
            }
            touchInfo.flag = 1; // ������־λ��1�������д�����������
        }
        else
        {
            touchInfo.flag = 0; // ������־λ��0���޴�������
        }
    }
    else
    {
        // I2C��ȡʧ��ʱ���㴥����Ϣ
        touchInfo.flag = 0;
        touchInfo.num = 0;
    }
}
