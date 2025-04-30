/*******************************************************************************
 * @note Copyright (C) Shanghai Panchip Microelectronics Co., Ltd.
 * All rights reserved.
 *
 * @file bsp.c
 * @brief 
 *
 * @history - V0.01, 2022-02-17, yangyaming, first implementation.
*******************************************************************************/

#include "bsp.h"

/*
 * @description	: bsp�����ʼ��
 * @param		: �� 
 * @return		: ��
 */
void bsp_init(void)
{
	bsp_rfspi_init();
	bsp_pn298_init();
	bsp_uart_init();
	bsp_timer_init();
}

