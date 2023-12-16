/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
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
#include "share_ware.h"
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

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void task_delay(uint16_t ms);
void start_freertos(void);
void comunication_task(void* pvParameters);
void imu_task(void* pvParameters);
/* USER CODE END FunctionPrototypes */

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void task_delay(uint16_t ms) {
    if (ms == 0u)
        return;

    float ticks = ms * configTICK_RATE_HZ / 1000.0f;
    if (ticks < 1.0f)
        vTaskDelay(1);
    else
        vTaskDelay(ticks);
}

void start_freertos() {
    xTaskCreate(comunication_task, "comunication_task", 128, NULL, 2, NULL);
    xTaskCreate(imu_task, "imu_task", 128, NULL, 2, NULL);
    vTaskStartScheduler();
}

void comunication_task(void* pvParameters) {
    taskENTER_CRITICAL();
    message_queue = xQueueCreate(100, sizeof(uint8_t));
    data_parser_init(&parser, 64);
    HAL_UART_Receive_IT(&huart1, &com_rx_data, 1);
    taskEXIT_CRITICAL();

    for (;;) {
        task_delay(1);

        // 接收并解析数据帧
        while (!parser.flag) {
            uint8_t data;
            if (xQueueReceive(message_queue, &data, 0) == pdTRUE) {
                parse(&parser, data);
            }
        }

        // 处理数据
        while (hdma_usart1_tx.State != HAL_DMA_STATE_READY) {}

        parser.flag = 0;
        process_data((uint8_t*)parser.buf);
    }
}

void imu_task(void* pvParameters) {
    icm_init();
    for (;;) {
        icm_get_raw_data(&icm_raw_data);
        task_delay(10);
    }
}

/* USER CODE END Application */
