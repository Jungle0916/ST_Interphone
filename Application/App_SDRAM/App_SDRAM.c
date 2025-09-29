/********************************************************
Copyright (c) 2025 Year, Dongguan City University., Ltd. All rights reserved.
FileName  : App_SDRAM.c 
Author    : zheng_kai_wen 
Version   : v0.0.0.0 
Date      : 2025.09.26 
Note      : none
History   : none 
*********************************************************/
/* Includes ------------------------------------------------------*/
#include "App_SDRAM.h"
#include "Com_Debug.h"

/* Private define ------------------------------------------------*/
/* Private typedef -----------------------------------------------*/
/* Public variables ----------------------------------------------*/

/* Public function prototypes ------------------------------------*/
/**
 ********************************************************************
 * name         : uint8_t SDRAM_Test(void)
 * description  : ����16λ�����ݿ��д�����ݣ��ٶ�ȡ����һһ���бȽϣ������8λ�����ݿ��д�룬
				  ������֤NBL0��NBL1�������ŵ������Ƿ�������
 * Input        : none
 * Output       : ����USART1������Խ��
 * Return       : SUCCESS - �ɹ���ERROR - ʧ��
 ********************************************************************/
uint8_t SDRAM_Test(void)
{
	uint32_t i = 0;		// ��������
	uint16_t ReadData = 0; 	// ��ȡ��������
	uint8_t  ReadData_8b;
	
	printf("STM32F429 SDRAM����\r\n");
	printf("���Կ�ʼ����16λ���ݿ��д������...\r\n");	
	
	for (i = 0; i < SDRAM_Size/2; i++)
	{
 		*(__IO uint16_t*) (SDRAM_BANK_ADDR + 2*i) = (uint16_t)i;		// д������
	}
	
	printf("д����ϣ���ȡ���ݲ��Ƚ�...\r\n");
	for(i = 0; i < SDRAM_Size/2;i++ )
	{
		ReadData = *(__IO uint16_t*)(SDRAM_BANK_ADDR + 2 * i );  // ��SDRAM��������	
		if( ReadData != (uint16_t)i )      //������ݣ�������ȣ���������,���ؼ��ʧ�ܽ����
		{
			printf("SDRAM����ʧ�ܣ���\r\n");
			return ERROR;	 // ����ʧ�ܱ�־
		}
	}
	
	printf("16λ���ݿ�ȶ�дͨ������8λ���ݿ��д������\r\n");
	for (i = 0; i < 255; i++)
	{
 		*(__IO uint8_t*) (SDRAM_BANK_ADDR + i) =  (uint8_t)i;
	}	
	printf("д����ϣ���ȡ���ݲ��Ƚ�...\r\n");
	for (i = 0; i < 255; i++)
	{
		ReadData_8b = *(__IO uint8_t*) (SDRAM_BANK_ADDR + i);
		if( ReadData_8b != (uint8_t)i )      //������ݣ�������ȣ���������,���ؼ��ʧ�ܽ����
		{
			printf("8λ���ݿ�ȶ�д����ʧ�ܣ���\r\n");
			printf("����NBL0��NBL1������\r\n");	
			return ERROR;	 // ����ʧ�ܱ�־
		}
	}		
	printf("8λ���ݿ�ȶ�дͨ��\r\n");
	printf("SDRAM��д����ͨ����ϵͳ����\r\n");
	return SUCCESS;	 // ���سɹ���־
}

/**
 ********************************************************************
 * name         : void SDRAM_Initialization_Sequence(SDRAM_HandleTypeDef *hsdram, FMC_SDRAM_CommandTypeDef *Command)
 * description  : SDRAM��ʼ�����У�����SDRAMоƬ�ĳ�ʼ�����̽�������
 *                ����ʱ��ʹ�ܡ�Ԥ��硢�Զ�ˢ�º�ģʽ�Ĵ������صȲ���
 * Input        : hsdram - SDRAM���ָ��
 *                Command - SDRAM����ṹ��ָ��
 * Output       : SDRAM��ʼ�����
 * Return       : none
 ********************************************************************/
void SDRAM_Initialization_Sequence(SDRAM_HandleTypeDef *hsdram, FMC_SDRAM_CommandTypeDef *Command)
{
  __IO uint32_t tmpmrd = 0;

  /* Configure a clock configuration enable command */
  Command->CommandMode        = FMC_SDRAM_CMD_CLK_ENABLE; // ����SDRAMʱ�� 
  Command->CommandTarget        = FMC_COMMAND_TARGET_BANK;  // ѡ��Ҫ���Ƶ�.bank
  Command->AutoRefreshNumber    = 1;
  Command->ModeRegisterDefinition = 0;

  HAL_SDRAM_SendCommand(hsdram, Command, SDRAM_TIMEOUT); // ���Ϳ�������
  HAL_Delay(1);   // ��ʱ�ȴ�

  /* Configure a PALL (precharge all) command */ 
  Command->CommandMode        = FMC_SDRAM_CMD_PALL;   // Ԥ�������.bank
  Command->CommandTarget        = FMC_COMMAND_TARGET_BANK;  // ѡ��Ҫ���Ƶ�.bank
  Command->AutoRefreshNumber    = 1;
  Command->ModeRegisterDefinition = 0;

  HAL_SDRAM_SendCommand(hsdram, Command, SDRAM_TIMEOUT);  // ���Ϳ�������

  /* Configure a Auto-Refresh command */ 
  Command->CommandMode        = FMC_SDRAM_CMD_AUTOREFRESH_MODE; // ʹ���Զ�ˢ��
  Command->CommandTarget        = FMC_COMMAND_TARGET_BANK;          // ѡ��Ҫ���Ƶ�.bank
  Command->AutoRefreshNumber    = 8;                                // �Զ�ˢ�´���
  Command->ModeRegisterDefinition = 0;

  HAL_SDRAM_SendCommand(hsdram, Command, SDRAM_TIMEOUT);  // ���Ϳ�������

  /* Program the external memory mode register */
  tmpmrd = (uint32_t)SDRAM_MODEREG_BURST_LENGTH_2         |
           SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL   |
           SDRAM_MODEREG_CAS_LATENCY_3           |
           SDRAM_MODEREG_OPERATING_MODE_STANDARD |
           SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;

  Command->CommandMode          = FMC_SDRAM_CMD_LOAD_MODE;  // ����ģʽ�Ĵ�������
  Command->CommandTarget        = FMC_COMMAND_TARGET_BANK;  // ѡ��Ҫ���Ƶ�.bank
  Command->AutoRefreshNumber    = 1;
  Command->ModeRegisterDefinition = tmpmrd;

  HAL_SDRAM_SendCommand(hsdram, Command, SDRAM_TIMEOUT);  // ���Ϳ�������
  
  HAL_SDRAM_ProgramRefreshRate(hsdram, 918);  // ����ˢ����
}

