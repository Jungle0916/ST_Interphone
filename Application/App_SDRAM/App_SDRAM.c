/********************************************************
Copyright (c) 2025 Dongguan City University. All rights reserved.
FileName  : App_SDRAM.c 
Author    : zheng_kai_wen 
Version   : v0.0.0.0 
Date      : 2025.09.26 
Note      : none
History   : 加入SDRAM内存池, 分配1MB用于LVGL内存分配 - 2025.10.01
*********************************************************/
/* Includes ------------------------------------------------------*/
#include "App_SDRAM.h"
#include "Com_Debug.h"
#include <string.h>

/* Private define ------------------------------------------------*/
#define SDRAM_START_ADDR (SDRAM_BANK_ADDR + 0x1000000)  // 从0xD1000000开始用于LVGL内存
#define SDRAM_POOL_SIZE  (1024 * 1024)                 // 为LVGL分配1MB内存

/* Private typedef -----------------------------------------------*/
// 定义内存块结构
typedef struct mem_block {
    uint32_t size;              // 内存块大小
    uint8_t is_free;            // 是否空闲
    struct mem_block* next;     // 下一个内存块
} mem_block_t;

/* Private variables ---------------------------------------------*/
static uint8_t sdram_pool[SDRAM_POOL_SIZE] __attribute__((at(SDRAM_START_ADDR)));
static mem_block_t* head_block = NULL;  // 内存块链表头

/* Public function prototypes ------------------------------------*/

/**
 ********************************************************************
 * name         : void sdram_init(void)
 * description  : 初始化SDRAM内存管理器
 * Input        : none
 * Output       : none
 * Return       : none
 ********************************************************************/
void sdram_init(void)
{
    // 初始化内存管理器
    head_block = (mem_block_t*)sdram_pool;
    head_block->size = SDRAM_POOL_SIZE - sizeof(mem_block_t);
    head_block->is_free = 1;
    head_block->next = NULL;
}

/**
 ********************************************************************
 * name         : void* sdram_malloc(size_t size)
 * description  : 在外部SDRAM中分配内存
 * Input        : size - 需要分配的内存大小
 * Output       : none
 * Return       : 分配的内存指针，如果失败返回NULL
 ********************************************************************/
void* sdram_malloc(size_t size)
{
    // 确保内存对齐（至少4字节对齐）
    size = (size + 3) & ~3;
    
    // 遍历内存块链表寻找合适的空闲块
    mem_block_t* current = head_block;
    while (current != NULL) {
        if (current->is_free && current->size >= size) {
            // 找到合适的空闲块
            if (current->size > size + sizeof(mem_block_t) + 4) {
                // 如果剩余空间足够大，分割内存块
                mem_block_t* new_block = (mem_block_t*)((uint8_t*)current + sizeof(mem_block_t) + size);
                new_block->size = current->size - size - sizeof(mem_block_t);
                new_block->is_free = 1;
                new_block->next = current->next;
                
                current->size = size;
                current->is_free = 0;
                current->next = new_block;
            } else {
                // 使用整个块
                current->is_free = 0;
            }
            
            // 返回用户可用的内存地址
            return (void*)((uint8_t*)current + sizeof(mem_block_t));
        }
        current = current->next;
    }
    
    // 没有找到合适的内存块
    return NULL;
}

/**
 ********************************************************************
 * name         : void sdram_free(void* ptr)
 * description  : 释放外部SDRAM中的内存
 * Input        : ptr - 要释放的内存指针
 * Output       : none
 * Return       : none
 ********************************************************************/
void sdram_free(void* ptr)
{
    if (ptr == NULL) return;
    
    // 获取内存块头部
    mem_block_t* block = (mem_block_t*)((uint8_t*)ptr - sizeof(mem_block_t));
    block->is_free = 1;
    
    // 合并相邻的空闲块
    mem_block_t* current = head_block;
    while (current != NULL) {
        if (current->is_free && current->next != NULL && current->next->is_free) {
            // 合并当前块和下一个块
            current->size += sizeof(mem_block_t) + current->next->size;
            current->next = current->next->next;
        } else {
            current = current->next;
        }
    }
}

/**
 ********************************************************************
 * name         : void* sdram_realloc(void* ptr, size_t size)
 * description  : 重新分配外部SDRAM中的内存
 * Input        : ptr - 原内存指针, size - 新的内存大小
 * Output       : none
 * Return       : 重新分配的内存指针，如果失败返回NULL
 ********************************************************************/
void* sdram_realloc(void* ptr, size_t size)
{
    if (ptr == NULL) {
        return sdram_malloc(size);
    }
    
    if (size == 0) {
        sdram_free(ptr);
        return NULL;
    }
    
    // 确保内存对齐
    size = (size + 3) & ~3;
    
    // 获取当前内存块信息
    mem_block_t* block = (mem_block_t*)((uint8_t*)ptr - sizeof(mem_block_t));
    
    if (size <= block->size) {
        // 新大小小于等于当前大小，直接返回
        return ptr;
    }
    
    // 需要更大的内存，分配新块并复制数据
    void* new_ptr = sdram_malloc(size);
    if (new_ptr != NULL) {
        // 复制原有数据
        memcpy(new_ptr, ptr, block->size);
        // 释放旧内存
        sdram_free(ptr);
    }
    
    return new_ptr;
}

/**
 ********************************************************************
 * name         : uint32_t sdram_get_free(void)
 * description  : 获取剩余内存大小
 * Input        : none
 * Output       : none
 * Return       : 剩余内存大小（字节）
 ********************************************************************/
uint32_t sdram_get_free(void)
{
    uint32_t free_size = 0;
    mem_block_t* current = head_block;
    
    while (current != NULL) {
        if (current->is_free) {
            free_size += current->size;
        }
        current = current->next;
    }
    
    return free_size;
}

/**
 ********************************************************************
 * name         : uint32_t sdram_get_used(void)
 * description  : 获取已使用内存大小
 * Input        : none
 * Output       : none
 * Return       : 已使用内存大小（字节）
 ********************************************************************/
uint32_t sdram_get_used(void)
{
    uint32_t used_size = 0;
    mem_block_t* current = head_block;
    
    while (current != NULL) {
        if (!current->is_free) {
            used_size += current->size;
        }
        current = current->next;
    }
    
    return used_size;
}

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

