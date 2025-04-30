/*******************************************************************************
 * @note Copyright (C) Shanghai Panchip Microelectronics Co., Ltd.
 * All rights reserved.
 *
 * @file bsp_i2c_hw.h
 * @brief 
 *
 * @history - V0.01, 2022-02-17, yangyaming, first implementation.
*******************************************************************************/
#ifndef __BSP_I2C_HW_H
#define __BSP_I2C_HW_H

#ifdef __cplusplus
extern "C" {
#endif

#include "cx32l003.h"
#include "cx32l003_hal_conf.h"




void bsp_i2c_hw_init(void);

uint8_t bsp_i2c_hw_read_byte(uint8_t dev, uint8_t reg);
void bsp_i2c_hw_read_bytes(uint8_t dev, uint8_t reg, uint8_t* buf, uint32_t len);
void bsp_i2c_hw_write_byte(uint8_t dev, uint8_t reg, uint8_t ubyte);
void bsp_i2c_hw_write_bytes(uint8_t dev, uint8_t reg, const uint8_t* buf, uint32_t len);

#ifdef __cplusplus
}
#endif

#endif // __LIB_DRIVER_RFIIC_PAN308_H
