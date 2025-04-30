/*******************************************************************************
 * @note      Copyright (C) Shanghai Panchip Microelectronics Co., Ltd.
 *            All rights reserved.
 *
 * @file      bsp_rfspi_sw.c
 * @brief     gpio 模拟spi 驱动
 *
 * @history - V1.0, 2022-08-08, yangyaming, first implementation.
*******************************************************************************/
#include "bsp_rfspi_sw.h"
/*  PAN742  PAN1026
 *   PF1      CSN
 *   PF3	  MISO	
 *	 PA0	  MOSI
 *   PA1	  CLK
 */
 
 void rfspi_pan742_write_byte(uint8_t ubyte)
{
	uint8_t i;  
    for(i = 0; i < 8; i++){
		__rfspi_pan742_write_clk(0); 
//		__rfspi_pan742_delay(1);
		if(ubyte & 0x80){
			__rfspi_pan742_write_mosi(1);
		}else{
			__rfspi_pan742_write_mosi(0);
		}
		ubyte <<= 1;
		__rfspi_pan742_write_clk(1);  
		__rfspi_pan742_delay(1);	
    } 
	__rfspi_pan742_write_clk(0);
	__rfspi_pan742_delay(5);
}

uint8_t rfspi_pan742_read_byte(void)
{
	uint8_t i;  
	uint8_t ret = 0;  
	for(i = 0; i < 8; i++) {  
		__rfspi_pan742_write_clk(0);  
		__rfspi_pan742_delay(1);   	
		__rfspi_pan742_write_clk(1);
		ret <<= 1;
		if(__rfspi_pan742_read_miso()){
			ret |= 0x01;
		} 
	}
	__rfspi_pan742_write_clk(0);
	__rfspi_pan742_delay(5);
	return ret;
}

void bsp_rfspi_init(void)
{

	LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);
	LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOF);
	

	//MOSI
	LL_GPIO_InitTypeDef GPIO_InitStruct;
	
	
 
  /* 输出模式选择*/
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pin = LL_GPIO_PIN_0;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
	
	LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	//CLK	
	GPIO_InitStruct.Pin = LL_GPIO_PIN_1;
	LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	//CS
	GPIO_InitStruct.Pin = LL_GPIO_PIN_1;
	LL_GPIO_Init(GPIOF, &GPIO_InitStruct);
	//MISO
	GPIO_InitStruct.Pin = LL_GPIO_PIN_3;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
	LL_GPIO_Init(GPIOF, &GPIO_InitStruct);
	
	__rfspi_pan742_write_mosi(0);
	__rfspi_pan742_write_clk(0);
	__rfspi_pan742_write_cs(1);
	
}
