/*******************************************************************************
 * @note      Copyright (C) Shanghai Panchip Microelectronics Co., Ltd.
 *            All rights reserved.
 *
 * @file      bsp_uart.h
 * @brief     uartÇý¶¯
 *
 * @history - V1.0, 2022-02-22, yangyaming, first implementation.
*******************************************************************************/

#ifndef __BSP_UART_H
#define __BSP_UART_H

#include "bsp.h"

void bsp_uart_init(void);
uint8_t bsp_uart_get_byte(uint8_t *data);

#endif
