/********************************************************
Copyright (c) 2025 Dongguan City University. All rights reserved.
FileName  : App_SDRAM.c 
Author    : zheng_kai_wen 
Version   : v0.0.0.0 
Date      : 2025.09.26 
Note      : none
History   : ����SDRAM�ڴ��, ����1MB����LVGL�ڴ���� - 2025.10.01
*********************************************************/
/* Includes ------------------------------------------------------*/
#include "App_SDRAM.h"
#include "Com_Debug.h"
#include <string.h>

/* Private define ------------------------------------------------*/
#define SDRAM_START_ADDR (SDRAM_BANK_ADDR + 0x1000000)  // ��0xD1000000��ʼ����LVGL�ڴ�
#define SDRAM_POOL_SIZE  (1024 * 1024)                 // ΪLVGL����1MB�ڴ�

/* Private typedef -----------------------------------------------*/
// �����ڴ��ṹ
typedef struct mem_block {
    uint32_t size;              // �ڴ���С
    uint8_t is_free;            // �Ƿ����
    struct mem_block* next;     // ��һ���ڴ��
} mem_block_t;

/* Private variables ---------------------------------------------*/
static uint8_t sdram_pool[SDRAM_POOL_SIZE] __attribute__((at(SDRAM_START_ADDR)));
static mem_block_t* head_block = NULL;  // �ڴ������ͷ

/* Public function prototypes ------------------------------------*/

/**
 ********************************************************************
 * name         : void sdram_init(void)
 * description  : ��ʼ��SDRAM�ڴ������
 * Input        : none
 * Output       : none
 * Return       : none
 ********************************************************************/
void sdram_init(void)
{
    // ��ʼ���ڴ������
    head_block = (mem_block_t*)sdram_pool;
    head_block->size = SDRAM_POOL_SIZE - sizeof(mem_block_t);
    head_block->is_free = 1;
    head_block->next = NULL;
}

/**
 ********************************************************************
 * name         : void* sdram_malloc(size_t size)
 * description  : ���ⲿSDRAM�з����ڴ�
 * Input        : size - ��Ҫ������ڴ��С
 * Output       : none
 * Return       : ������ڴ�ָ�룬���ʧ�ܷ���NULL
 ********************************************************************/
void* sdram_malloc(size_t size)
{
    // ȷ���ڴ���루����4�ֽڶ��룩
    size = (size + 3) & ~3;
    
    // �����ڴ������Ѱ�Һ��ʵĿ��п�
    mem_block_t* current = head_block;
    while (current != NULL) {
        if (current->is_free && current->size >= size) {
            // �ҵ����ʵĿ��п�
            if (current->size > size + sizeof(mem_block_t) + 4) {
                // ���ʣ��ռ��㹻�󣬷ָ��ڴ��
                mem_block_t* new_block = (mem_block_t*)((uint8_t*)current + sizeof(mem_block_t) + size);
                new_block->size = current->size - size - sizeof(mem_block_t);
                new_block->is_free = 1;
                new_block->next = current->next;
                
                current->size = size;
                current->is_free = 0;
                current->next = new_block;
            } else {
                // ʹ��������
                current->is_free = 0;
            }
            
            // �����û����õ��ڴ��ַ
            return (void*)((uint8_t*)current + sizeof(mem_block_t));
        }
        current = current->next;
    }
    
    // û���ҵ����ʵ��ڴ��
    return NULL;
}

/**
 ********************************************************************
 * name         : void sdram_free(void* ptr)
 * description  : �ͷ��ⲿSDRAM�е��ڴ�
 * Input        : ptr - Ҫ�ͷŵ��ڴ�ָ��
 * Output       : none
 * Return       : none
 ********************************************************************/
void sdram_free(void* ptr)
{
    if (ptr == NULL) return;
    
    // ��ȡ�ڴ��ͷ��
    mem_block_t* block = (mem_block_t*)((uint8_t*)ptr - sizeof(mem_block_t));
    block->is_free = 1;
    
    // �ϲ����ڵĿ��п�
    mem_block_t* current = head_block;
    while (current != NULL) {
        if (current->is_free && current->next != NULL && current->next->is_free) {
            // �ϲ���ǰ�����һ����
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
 * description  : ���·����ⲿSDRAM�е��ڴ�
 * Input        : ptr - ԭ�ڴ�ָ��, size - �µ��ڴ��С
 * Output       : none
 * Return       : ���·�����ڴ�ָ�룬���ʧ�ܷ���NULL
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
    
    // ȷ���ڴ����
    size = (size + 3) & ~3;
    
    // ��ȡ��ǰ�ڴ����Ϣ
    mem_block_t* block = (mem_block_t*)((uint8_t*)ptr - sizeof(mem_block_t));
    
    if (size <= block->size) {
        // �´�СС�ڵ��ڵ�ǰ��С��ֱ�ӷ���
        return ptr;
    }
    
    // ��Ҫ������ڴ棬�����¿鲢��������
    void* new_ptr = sdram_malloc(size);
    if (new_ptr != NULL) {
        // ����ԭ������
        memcpy(new_ptr, ptr, block->size);
        // �ͷž��ڴ�
        sdram_free(ptr);
    }
    
    return new_ptr;
}

/**
 ********************************************************************
 * name         : uint32_t sdram_get_free(void)
 * description  : ��ȡʣ���ڴ��С
 * Input        : none
 * Output       : none
 * Return       : ʣ���ڴ��С���ֽڣ�
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
 * description  : ��ȡ��ʹ���ڴ��С
 * Input        : none
 * Output       : none
 * Return       : ��ʹ���ڴ��С���ֽڣ�
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

