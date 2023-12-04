#ifndef CONTROL_DATA_PARSER_H
#define CONTROL_DATA_PARSER_H

#include "main.h"

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

#endif  // CONTROL_DATA_PARSER_H
