/*******************************************************************************
 * @note Copyright (C) Shanghai Panchip Microelectronics Co., Ltd.
 * All rights reserved.
 *
 * @file lib_driver_delay_pan742.c
 * @brief 
 *
 * @history - V0.01, 2022-08-08, yangyaming, first implementation.
*******************************************************************************/

#include "bsp_delay.h"

/*
 * @description	: delay函数
 * @param		: delay的值 
 * @return		: 无
 */
__ASM void __delay_pan742(uint32_t cycle)
{
__delay_pan742_loop
    SUBS    r0,r0,#1
    BCS     __delay_pan742_loop
    BX      lr
}
