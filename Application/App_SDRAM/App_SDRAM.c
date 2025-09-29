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
 * description  : 先以16位的数据宽度写入数据，再读取出来一一进行比较，随后以8位的数据宽度写入，
				  用以验证NBL0和NBL1两个引脚的连接是否正常。
 * Input        : none
 * Output       : 串口USART1输出测试结果
 * Return       : SUCCESS - 成功，ERROR - 失败
 ********************************************************************/
uint8_t SDRAM_Test(void)
{
	uint32_t i = 0;		// 计数变量
	uint16_t ReadData = 0; 	// 读取到的数据
	uint8_t  ReadData_8b;
	
	printf("STM32F429 SDRAM测试\r\n");
	printf("测试开始，以16位数据宽度写入数据...\r\n");	
	
	for (i = 0; i < SDRAM_Size/2; i++)
	{
 		*(__IO uint16_t*) (SDRAM_BANK_ADDR + 2*i) = (uint16_t)i;		// 写入数据
	}
	
	printf("写入完毕，读取数据并比较...\r\n");
	for(i = 0; i < SDRAM_Size/2;i++ )
	{
		ReadData = *(__IO uint16_t*)(SDRAM_BANK_ADDR + 2 * i );  // 从SDRAM读出数据	
		if( ReadData != (uint16_t)i )      //检测数据，若不相等，跳出函数,返回检测失败结果。
		{
			printf("SDRAM测试失败！！\r\n");
			return ERROR;	 // 返回失败标志
		}
	}
	
	printf("16位数据宽度读写通过，以8位数据宽度写入数据\r\n");
	for (i = 0; i < 255; i++)
	{
 		*(__IO uint8_t*) (SDRAM_BANK_ADDR + i) =  (uint8_t)i;
	}	
	printf("写入完毕，读取数据并比较...\r\n");
	for (i = 0; i < 255; i++)
	{
		ReadData_8b = *(__IO uint8_t*) (SDRAM_BANK_ADDR + i);
		if( ReadData_8b != (uint8_t)i )      //检测数据，若不相等，跳出函数,返回检测失败结果。
		{
			printf("8位数据宽度读写测试失败！！\r\n");
			printf("请检查NBL0和NBL1的连接\r\n");	
			return ERROR;	 // 返回失败标志
		}
	}		
	printf("8位数据宽度读写通过\r\n");
	printf("SDRAM读写测试通过，系统正常\r\n");
	return SUCCESS;	 // 返回成功标志
}

/**
 ********************************************************************
 * name         : void SDRAM_Initialization_Sequence(SDRAM_HandleTypeDef *hsdram, FMC_SDRAM_CommandTypeDef *Command)
 * description  : SDRAM初始化序列，按照SDRAM芯片的初始化流程进行配置
 *                包括时钟使能、预充电、自动刷新和模式寄存器加载等步骤
 * Input        : hsdram - SDRAM句柄指针
 *                Command - SDRAM命令结构体指针
 * Output       : SDRAM初始化完成
 * Return       : none
 ********************************************************************/
void SDRAM_Initialization_Sequence(SDRAM_HandleTypeDef *hsdram, FMC_SDRAM_CommandTypeDef *Command)
{
  __IO uint32_t tmpmrd = 0;

  /* Configure a clock configuration enable command */
  Command->CommandMode        = FMC_SDRAM_CMD_CLK_ENABLE; // 开启SDRAM时钟 
  Command->CommandTarget        = FMC_COMMAND_TARGET_BANK;  // 选择要控制的.bank
  Command->AutoRefreshNumber    = 1;
  Command->ModeRegisterDefinition = 0;

  HAL_SDRAM_SendCommand(hsdram, Command, SDRAM_TIMEOUT); // 发送控制命令
  HAL_Delay(1);   // 延时等待

  /* Configure a PALL (precharge all) command */ 
  Command->CommandMode        = FMC_SDRAM_CMD_PALL;   // 预充电所有.bank
  Command->CommandTarget        = FMC_COMMAND_TARGET_BANK;  // 选择要控制的.bank
  Command->AutoRefreshNumber    = 1;
  Command->ModeRegisterDefinition = 0;

  HAL_SDRAM_SendCommand(hsdram, Command, SDRAM_TIMEOUT);  // 发送控制命令

  /* Configure a Auto-Refresh command */ 
  Command->CommandMode        = FMC_SDRAM_CMD_AUTOREFRESH_MODE; // 使能自动刷新
  Command->CommandTarget        = FMC_COMMAND_TARGET_BANK;          // 选择要控制的.bank
  Command->AutoRefreshNumber    = 8;                                // 自动刷新次数
  Command->ModeRegisterDefinition = 0;

  HAL_SDRAM_SendCommand(hsdram, Command, SDRAM_TIMEOUT);  // 发送控制命令

  /* Program the external memory mode register */
  tmpmrd = (uint32_t)SDRAM_MODEREG_BURST_LENGTH_2         |
           SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL   |
           SDRAM_MODEREG_CAS_LATENCY_3           |
           SDRAM_MODEREG_OPERATING_MODE_STANDARD |
           SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;

  Command->CommandMode          = FMC_SDRAM_CMD_LOAD_MODE;  // 加载模式寄存器命令
  Command->CommandTarget        = FMC_COMMAND_TARGET_BANK;  // 选择要控制的.bank
  Command->AutoRefreshNumber    = 1;
  Command->ModeRegisterDefinition = tmpmrd;

  HAL_SDRAM_SendCommand(hsdram, Command, SDRAM_TIMEOUT);  // 发送控制命令
  
  HAL_SDRAM_ProgramRefreshRate(hsdram, 918);  // 设置刷新率
}

