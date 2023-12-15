#include "share_ware.h"

// 共享变量
__IO uint8_t com_tx_data[COM_DATA_TX_SIZE];  // uart通信发送缓冲
__IO uint8_t com_rx_data[COM_DATA_RX_SIZE];  // uart通信接收缓冲
__IO uint8_t com_rx_len;                     // uart通信接收长度

QueueHandle_t message_queue; // 消息队列句柄

// 共享结构体
Data_Parser parser;
ICM_Raw_Data icm_raw_data;
