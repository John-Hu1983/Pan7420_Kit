/*******************************************************************************
 * @note Copyright (C) Shanghai Panchip Microelectronics Co., Ltd.
 * All rights reserved.
 *
 * @file bsp_i2c_hw.c
 * @brief 
 *
 * @history - V0.01, 2022-02-17, yangyaming, first implementation.
*******************************************************************************/
#include "bsp_i2c_hw.h"

#define __bsp_i2c_hw_mutex_lock()       __disable_irq()
#define __bsp_i2c_hw_mutex_unlock()     __enable_irq()

I2C_HandleTypeDef I2C_InitStructure;

/*
 * @description	: i2c外设使能
 * @param		: 无 
 * @return		: 无
 */
static void __bsp_i2c_hw_rcc_init(void)
{
	//外设时钟使能
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_I2C_CLK_ENABLE();
}
/*
 * @description	: i2c gpio初始化
 * @param		: 无 
 * @return		: 无
 */
static void __bsp_i2c_hw_gpio_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	/*
	 * SDA 配置
	 */
	GPIO_InitStructure.Pin = GPIO_PIN_5;
	GPIO_InitStructure.Mode = GPIO_MODE_AF;
	GPIO_InitStructure.Alternate = GPIO_AF4_I2C_SCL;
	GPIO_InitStructure.OpenDrain = GPIO_OPENDRAIN;
	GPIO_InitStructure.Debounce.Enable = GPIO_DEBOUNCE_DISABLE;
	GPIO_InitStructure.SlewRate = GPIO_SLEW_RATE_LOW;
	GPIO_InitStructure.DrvStrength = GPIO_DRV_STRENGTH_LOW;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/*
	 * SCL 配置
	 */
	GPIO_InitStructure.Pin = GPIO_PIN_4;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
}
/*
 * @description	: i2c外设初始化
 * @param		: 无 
 * @return		: 无
 */
static void __bsp_i2c_hw_init(void)	
{
	I2C_InitStructure.Instance = I2C;
	I2C_InitStructure.Init.master = I2C_MASTER_MODE_ENABLE;
	I2C_InitStructure.Init.slave = I2C_SLAVE_MODE_DISABLE;
	I2C_InitStructure.Mode = HAL_I2C_MODE_MASTER;
	I2C_InitStructure.Init.broadack = I2C_BROAD_ACK_DISABLE;
	I2C_InitStructure.Init.speedclock = 500;
	I2C_InitStructure.State = HAL_I2C_STATE_RESET;

	HAL_I2C_Init(&I2C_InitStructure);
}

/*
 * @description	: i2c初始化
 * @param		: 无 
 * @return		: 无
 */
void bsp_i2c_hw_init(void)
{
	__bsp_i2c_hw_rcc_init();
	__bsp_i2c_hw_gpio_init();
	__bsp_i2c_hw_init();
}
/*
 * @description	: 读取i2c单字节
 * @param		: 无 
 * @return		: 读取到的字节
 */
uint8_t bsp_i2c_hw_read_byte(uint8_t dev, uint8_t reg)
{
	uint8_t ret;
    __bsp_i2c_hw_mutex_lock();
	HAL_I2C_Master_Receive(&I2C_InitStructure, dev, &reg, 1, &ret, 1);
	__bsp_i2c_hw_mutex_unlock();
	return ret;
}
/*
 * @description	: 读取i2c多字节
 * @param		: 无 
 * @return		: 无
 */
void bsp_i2c_hw_read_bytes(uint8_t dev, uint8_t reg, uint8_t* buf, uint32_t len)
{
    __bsp_i2c_hw_mutex_lock();;
	HAL_I2C_Master_Receive(&I2C_InitStructure, dev, &reg, 1, buf, len);
	__bsp_i2c_hw_mutex_unlock();
}
/*
 * @description	: 写i2c单字节
 * @param		: 无 
 * @return		: 无
 */
void bsp_i2c_hw_wrtie_byte(uint8_t dev, uint8_t reg, uint8_t ubyte)
{
	bsp_i2c_hw_write_bytes(dev, reg, &ubyte, 1);
}

/*
 * @description	: 写i2c多字节
 * @param		: 无 
 * @return		: 无
 */
uint8_t data[] = {0};
void bsp_i2c_hw_write_bytes(uint8_t dev, uint8_t reg, const uint8_t* buf, uint32_t len)
{
	uint8_t i;
	
    __bsp_i2c_hw_mutex_lock();
	data[0] = reg;
	for(i = 0; i < len; i++){
		data[i+1] = buf[i];
	}
    HAL_I2C_Master_Transmit(&I2C_InitStructure, dev, data, len + 1);
	__bsp_i2c_hw_mutex_unlock();
}
