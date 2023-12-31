#include "share_ware.h"
#include "FreeRTOS.h"
#include "task.h"

static void task_delay(uint16_t ms) {
    if (ms == 0u)
        return;

    float ticks = ms * configTICK_RATE_HZ / 1000.0f;
    if (ticks < 1.0f)
        vTaskDelay(1);
    else
        vTaskDelay(ticks);
}

static void comunication_task(void* pvParameters) {
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
        while (huart1.gState != HAL_UART_STATE_READY);

        parser.flag = 0;
        process_data((uint8_t*)parser.buf);
    }
}

static void imu_task(void* pvParameters) {
    icm_init();
    for (;;) {
        icm_get_raw_data(&icm_raw_data);
        task_delay(10);
    }
}

static void gpio_test_task(void* pvParameters) {
    for (;;) {
        uint8_t cnt = 0;
        if (!HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3))
            ++cnt;
        if (!HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4))
            ++cnt;
        if (!HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5))
            ++cnt;

        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, !(cnt % 2));
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, cnt < 2);
        task_delay(10);
    }
}

void start_tasks() {
    xTaskCreate(comunication_task, "comunication_task", 128, NULL, 2, NULL);
    xTaskCreate(imu_task, "imu_task", 128, NULL, 2, NULL);
    xTaskCreate(gpio_test_task, "gpio_test_task", 128, NULL, 2, NULL);
    vTaskStartScheduler();
}
