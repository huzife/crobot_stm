#ifndef CONTROL_DATA_COM_H
#define CONTROL_DATA_COM_H

#include "main.h"

#define COM_DATA_TX_SIZE 64u
#define COM_DATA_RX_SIZE 64u

// 数据帧功能码
typedef enum {
    NONE = 0,
    SET_SPEED,
    GET_SPEED,
    GET_IMU_TEMPERATURE,
    GET_IMU
} Function_Code;

// 数据解析结构体
typedef struct {
    __IO uint16_t buf_len;  // 缓存长度
    __IO uint16_t data_len; // 当前数据长度
    __IO uint8_t *buf;      // 缓存地址
    __IO uint8_t flag;      // 解析完成标志
    __IO uint8_t FE;        // 帧头第一字节标志
    __IO uint8_t FH;        // 接收完整帧头标志
} Data_Parser;

void data_parser_init(Data_Parser *parser, uint16_t b_len);    // 解析器初始化函数
void parse(Data_Parser *parser, uint8_t data);   // 解析数据函数
void process_data(uint8_t *buf); // 帧数据处理函数

#endif  // CONTROL_DATA_COM_H
