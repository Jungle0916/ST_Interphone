/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
TickType_t tick_count = 0;
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for Heartbeat_Led */
osThreadId_t Heartbeat_LedHandle;
const osThreadAttr_t Heartbeat_Led_attributes = {
  .name = "Heartbeat_Led",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for Peripheral_Init */
osThreadId_t Peripheral_InitHandle;
const osThreadAttr_t Peripheral_Init_attributes = {
  .name = "Peripheral_Init",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for Task_LVGL */
osThreadId_t Task_LVGLHandle;
const osThreadAttr_t Task_LVGL_attributes = {
  .name = "Task_LVGL",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityLow,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void Task_Heartbeat_Led(void *argument);
void Task_Peripheral_Init(void *argument);
void Task_LVGL_Tick(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of Heartbeat_Led */
  Heartbeat_LedHandle = osThreadNew(Task_Heartbeat_Led, NULL, &Heartbeat_Led_attributes);

  /* creation of Peripheral_Init */
  Peripheral_InitHandle = osThreadNew(Task_Peripheral_Init, NULL, &Peripheral_Init_attributes);

  /* creation of Task_LVGL */
  Task_LVGLHandle = osThreadNew(Task_LVGL_Tick, NULL, &Task_LVGL_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
	  // 获取并打印系统tick计数
	  tick_count = xTaskGetTickCount();
	  osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_Task_Heartbeat_Led */
/**
* @brief Function implementing the Task_Heartbeat_ thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Task_Heartbeat_Led */
__weak void Task_Heartbeat_Led(void *argument)
{
  /* USER CODE BEGIN Task_Heartbeat_Led */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END Task_Heartbeat_Led */
}

/* USER CODE BEGIN Header_Task_Peripheral_Init */
/**
* @brief Function implementing the Peripheral_Init thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Task_Peripheral_Init */
__weak void Task_Peripheral_Init(void *argument)
{
  /* USER CODE BEGIN Task_Peripheral_Init */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END Task_Peripheral_Init */
}

/* USER CODE BEGIN Header_Task_LVGL_Tick */
/**
* @brief Function implementing the Task_LVGL thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Task_LVGL_Tick */
__weak void Task_LVGL_Tick(void *argument)
{
  /* USER CODE BEGIN Task_LVGL_Tick */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END Task_LVGL_Tick */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

