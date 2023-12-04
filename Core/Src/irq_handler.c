#include "share_ware.h"
#include <string.h>

/**
 *  函数功能：DMA2通道2传输完成回调函数
 *  入口参数：None
 *  返 回 值：None
 */
//void dma2_stream2_Handler() {
//    com_rx_done = 1;
//}

/**
 *  函数功能：DMA2通道7传输完成回调函数
 *  入口参数：None
 *  返 回 值：None
 */
//void dma2_stream7_Handler() {
//    com_tx_done = 1;
//}

/**
 *  函数功能：uart1空闲中断回调函数
 *  入口参数：None
 *  返 回 值：None
 */
void uart1_irq_handler() {
    if (RESET != __HAL_UART_GET_FLAG(&huart1, UART_FLAG_IDLE)) {
        __HAL_UART_CLEAR_IDLEFLAG(&huart1);
        
        HAL_UART_DMAStop(&huart1);
        
        // 读取CNDTR寄存器的值，计算接收到的数据长度
        com_rx_len = COM_DATA_RX_SIZE - __HAL_DMA_GET_COUNTER(&hdma_usart1_rx);

        // 将数据发送至消息队列
        if (message_queue != NULL) {
            BaseType_t xHigherPriorityTaskWoken;
            for (int i = 0; i < com_rx_len; i++) {
                xQueueSendFromISR(message_queue, (uint8_t*)(com_rx_data + i), &xHigherPriorityTaskWoken);
            }
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }
        
        HAL_UART_Receive_DMA(&huart1, (uint8_t*)com_rx_data, COM_DATA_RX_SIZE);
    }
}
