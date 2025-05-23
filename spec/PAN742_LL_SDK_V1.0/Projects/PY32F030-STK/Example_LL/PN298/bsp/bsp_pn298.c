/*******************************************************************************
 * @note      Copyright (C) Shanghai Panchip Microelectronics Co., Ltd.
 *            All rights reserved.
 *
 * @file      bsp_pn298.c
 * @brief     pn298 RF驱动
 *
 * @history - V1.0, 2022-02-17, yangyaming, first implementation.
*******************************************************************************/

#include "bsp_pn298.h"


/*
 * @description	: 向PN298寄存器写入一个数据
 * @param		: reg
				：data
 * @return		: 无
 */
void bsp_pn298_write_reg(uint8_t reg, uint8_t data)
{
	__rfspi_pan742_write_cs(0);
	rfspi_pan742_write_byte(reg);
	rfspi_pan742_write_byte(data);
	__rfspi_pan742_write_cs(1);
}
/*
 * @description	: 从PN298寄存器读取一个数据
 * @param		: reg
 * @return		: 读取到的数据
 */
uint8_t bsp_pn298_read_reg(uint8_t reg)
{
    uint8_t temp;
	__rfspi_pan742_write_cs(0);
	rfspi_pan742_write_byte(reg);
	temp= rfspi_pan742_read_byte();
	__rfspi_pan742_write_cs(1);
    return temp;
}
/*
 * @description	: 从PN298寄存器读取多个数据
 * @param		: reg
				：*pbuf
				：length
 * @return		: 无
 */
void bsp_pn298_write_buf(uint8_t reg,uint8_t *pbuf, uint8_t len)
{
	uint8_t i;
	__rfspi_pan742_write_cs(0);
	rfspi_pan742_write_byte(reg);
	
	for(i = 0; i < len; i++){
		rfspi_pan742_write_byte(pbuf[i]);
	}
	__rfspi_pan742_write_cs(1);
}
/*
 * @description	: 从PN298寄存器读取多个数据
 * @param		: reg
				：*pbuf
				：length
 * @return		: 无
 */
void bsp_pn298_read_buf(uint8_t reg, uint8_t *pbuf, uint8_t len)
{
	uint8_t i;
	__rfspi_pan742_write_cs(0);
	rfspi_pan742_write_byte(reg);
	for(i = 0; i < len; i++){
		pbuf[i] = rfspi_pan742_read_byte();
	}
	__rfspi_pan742_write_cs(1);
}
/*
 * @description	: 打开ce
 * @param		: 无
 * @return		: 无
 */

void bsp_pn298_ce_enable(void)
{
	bsp_pn298_write_reg(CE_ON, 0x00);
}
/*
 * @description	: 关闭ce
 * @param		: 无
 * @return		: 无
 */
void bsp_pn298_ce_disable(void)
{
	bsp_pn298_write_reg(CE_OFF, 0x00);
}
/*
 * @description	: 获取PN298的状态
 * @param		: 无
 * @return		: 无
 */
uint8_t	bsp_pan1026_get_status(void)
{
	return bsp_pn298_read_reg(0x07);
}

/*
 * @description	: 清除状态
 * @param		: 无
 * @return		: 无
 */
void bsp_pn298_clear_status(void)
{
	bsp_pn298_write_reg(W_REGISTER + 0x07, 0x70);

}
/*
 * @description	: 清除rx fifo
 * @param		: 无
 * @return		: 无
 */
void bsp_pn298_flush_rx_fifo(void)
{
	bsp_pn298_write_reg(FLUSH_RX, 0x00);
}
/*
 * @description	: 打开ce
 * @param		: 无
 * @return		: 无
 */
void bsp_pn298_flush_tx_fifo(void)
{
	bsp_pn298_write_reg(FLUSH_TX, 0x00);
}
/*
 * @description	: 清除rx，tx fifo
 * @param		: 无
 * @return		: 无
 */
void bsp_pn298_clear_fifo(void)
{
	bsp_pn298_write_reg(FLUSH_TX, 0x00);
	bsp_pn298_write_reg(FLUSH_RX, 0x00);
}

/*
 * @description	: 设置tx模式
 * @param		: 无
 * @return		: 无
 */
void bsp_pn298_tx_mode(void)
{
	uint8_t temp;
	bsp_pn298_ce_disable();
	temp = bsp_pn298_read_reg(0x00);
	temp &= ~(0x01);
	bsp_pn298_write_reg(W_REGISTER + 0x00, temp);
	
	bsp_pn298_flush_tx_fifo();
	bsp_pn298_flush_rx_fifo();
	bsp_pn298_clear_status();
	
	bsp_pn298_ce_enable();
}
/*
 * @description	: 设置为rx模式
 * @param		: 无
 * @return		: 无
 */
void bsp_pn298_rx_mode(void)
{
	uint8_t temp;
	temp = bsp_pn298_read_reg(0x00);
	temp |= 0x01;
	bsp_pn298_write_reg(W_REGISTER + 0x00, temp);
}
/*
 * @description	: 设置发射功率
 * @param		: power
 * @return		: 无
 */
void bsp_pn298_set_power(uint16_t power)
{
	uint8_t data[3];
    data[0] = 0x00;
    data[1] = (uint8_t)(power >> 8);
    data[2] = (uint8_t)(power);
    bsp_pn298_write_buf(W_REGISTER + 0x1a, data, 3);
}

/*
 * @description	: 设置信道
 * @param		: chn
 * @return		: 无
 */
void bsp_pn298_set_channel(uint8_t chn)
{
	chn += 64;
	bsp_pn298_write_reg(W_REGISTER + 0x05, chn);
}
/*
 * @description	: 设置模式
 * @param		: mode
 * @return		: 无
 */
void bsp_pn298_set_mode(uint8_t mode)
{
	uint8_t data[5];
    uint8_t temp;
    /* nordic模式关闭扰码 */
	temp = bsp_pn298_read_reg(0x19);
	if((mode == PN298_NORDIC_NORMAL_MODE) || (mode == PN298_NORDIC_ENHANCE_MODE)){
		temp &= ~(0x01);
	}else{
		temp |= 0x01;
	}
	
    bsp_pn298_write_reg(W_REGISTER + 0x19, temp);
    
    /* nordic增强型有单独的时间配置 */
    if(mode == PN298_NORDIC_ENHANCE_MODE){
		data[0] = 0xcc; 
		data[1] = 0x41; 
		data[2] = 0x04;     
    }else{
        data[0] = 0xd3; 
		data[1] = 0x59; 
		data[2] = 0x0c;
    }
    data[3] = 0x50; 
	data[4] = 0x9c;
    bsp_pn298_write_buf(W_REGISTER + 0x1f, data, 5);
  
    /* 设置模式 */
	temp = bsp_pn298_read_reg(0x00);
    temp &= ~(0x03 << 6);
    temp |= (((uint8_t)mode & PN298_MODE_MSK) << 2);
    bsp_pn298_write_reg(W_REGISTER + 0x00, temp); 
    
    /* 增强型与非增强型有不同的配置 */
	temp = bsp_pn298_read_reg(0x01);
    if(((uint8_t)mode & 0x01) == 0){
        temp &= ~(0x01);
        bsp_pn298_write_reg(W_REGISTER + 0x04, 0x00);
    }else{
        temp |= 0x01;
        bsp_pn298_write_reg(W_REGISTER + 0x04, 0x01);  /* 若失败不自动重传 */
    }
    bsp_pn298_write_reg(W_REGISTER + 0x01, temp);
}

/*
 * @description	: 设置通信地址
 * @param		: reg
				  *rf_addr_array
				  addr_len
 * @return		: 无
 */
void bsp_pn298_set_addr(uint8_t reg, uint8_t *rf_addr_array, uint8_t addr_len)
{
	bsp_pn298_write_buf(W_REGISTER + reg, rf_addr_array, addr_len);
}
/*
 * @description	: 设置payload长度
 * @param		: len
 * @return		: 无
 */
void bsp_pn298_set_rx_payload_len(uint8_t len)
{
	if(len <= 64){
		bsp_pn298_write_reg(W_REGISTER + 0x11, len);
	}
}
/*
 * @description	: 反位序和字节序
 * @param		: *buf
				  len
 * @return		: 无
 */
void bsp_pn298_byte_bit_decode(uint8_t* buf, uint8_t len)
{
    uint8_t data1;
    uint8_t data2;
    uint8_t i;
    uint8_t j;
    uint8_t* p1;
    uint8_t* p2;
    
    i = len >> 1;
    p1 = buf;
    p2 = buf + len - 1;

    if((len & 1) != 0){
        /* len奇数, 中间一个字节下面while里不会处理, 先额外处理 */
        if(len > 1){
            buf = buf + i;               
        }
        data1 = *buf;
        
        *buf = 0;
        for(j = 0; j < 7; j++){
            *buf += ((data1 >> j) & 0x01);
            *buf = *buf << 1;
        }
        *buf += (data1 >> 7);
    }
    
    while(i--){
        data1 = *p1;
        data2 = *p2;
        *p1 = *p2 = 0;
        for(j = 0; j < 7; j++){
            *p1 += ((data2 >> j) & 0x01);
            *p1 = *p1 << 1;
            *p2 += ((data1 >> j) & 0x01);
            *p2 = *p2 << 1;
        }
        *p1 += (data2 >> 7);
        *p2 += (data1 >> 7);
        p1++;
        p2--;
    }
}
/*
 * @description	: 发送数据
 * @param		: data 
				  len
 * @return		: 无
 */
void bsp_pn298_tx_data(uint8_t *data, uint8_t len)
{
	bsp_pn298_write_buf(W_TX_PAYLOAD, data, len);
	while(!((bsp_pn298_read_reg(RF_STATUS) & TX_DS_FLAG) == TX_DS_FLAG));
	bsp_pn298_flush_tx_fifo();
	bsp_pn298_clear_status();
}

uint8_t status;

uint16_t cnt = 0;

void bsp_pn298_tx_data_ack(uint8_t *data, uint8_t *ack_data, uint8_t len)
{
	bsp_pn298_write_buf(W_TX_PAYLOAD, data, len);
	delay_ms(2);
	status = (bsp_pn298_read_reg(RF_STATUS) & 0x70);
//	while(!((bsp_pn298_read_reg(RF_STATUS) & RX_TX_FLAG) == RX_TX_FLAG));
	while(status == 0x00);
	
	
	if(status == RX_TX_FLAG){
		bsp_pn298_read_buf(R_RX_PAYLOAD, ack_data, len);	//读取ACK数据
		printf("Tx cnt:%d\r\n", ++cnt);
	}
	bsp_pn298_flush_rx_fifo();
	bsp_pn298_flush_tx_fifo();
	bsp_pn298_clear_status();
}
/*
 * @description	: 接收数据
 * @param		: *data
				  len
 * @return		: 0 或 1
 */
uint8_t	bsp_pn298_rx_data(uint8_t *data,uint8_t len)
{
	if(!((bsp_pn298_read_reg(RF_STATUS) & RX_DR_FLAG) == RX_DR_FLAG)){
		return 0;
	}
	bsp_pn298_read_buf(R_RX_PAYLOAD, data, len);
	bsp_pn298_byte_bit_decode(data, len);
	bsp_pn298_flush_rx_fifo();
	bsp_pn298_clear_status();
	return 1;
}
/*
 * @description	: 接收数据，并待ack
 * @param		: *rx_data
				  *ack_data
				  len
				  ack
 * @return		: 0 或 1
 */

uint8_t bsp_pn298_rx_data_ack_fix(uint8_t *rx_data, uint8_t *ack_data, uint8_t len, uint8_t ack)
{
	if((bsp_pn298_read_reg(RF_STATUS) & RX_DR_FLAG) == RX_DR_FLAG){
        if(ack){
            bsp_pn298_write_reg(FLUSH_TX, 0);						// 清除发送缓冲
            bsp_pn298_write_buf(W_ACK_PAYLOAD, ack_data, len);	// 发送ACK数据
        }
		//delay_us(300);
        bsp_pn298_read_buf(R_RX_PAYLOAD, rx_data, len);			// 接收数据
        bsp_pn298_write_reg(FLUSH_RX, 0);							// 清除接收缓冲
        bsp_pn298_write_reg(W_REGISTER + RF_STATUS, 0x70);      	//清除状态
        return 1;
    }
	return 0;
}
/*
 * @description	: 1.8v寄存器初始化
 * @param		: 无
 * @return		: 无
 */
void __bsp_pn298_1v8_reg_init(void)
{
	uint8_t data[5];
	uint8_t demod_cal;
	
	data[0] = 0x20; 
    /* data[1] = 0xd0; data[2] = 0xff; */    /* 电流大 接收灵敏度好 */
    data[1] = 0xc0; data[2] = 0xfb;          /* 电流小 接收灵敏度差1dB */
    bsp_pn298_write_buf(W_REGISTER + 0x03, data, 3);
    
	data[0] = 0x48; 
	data[1] = 0x2a; 
	data[2] = 0xfd;
   	bsp_pn298_write_buf(W_REGISTER + 0x06, data, 3);
   
	data[0] = 0xfd; 
	data[1] = 0x3f;
    bsp_pn298_write_buf(W_REGISTER + 0x0e, data, 2);
    
	data[0] = 0x8b;
	data[1] = 0xc9; //A版本：0xc9; B版本：0x09
	data[2] = 0x5f;
    bsp_pn298_write_buf(W_REGISTER + 0x18, data, 3);
    
    demod_cal = 0x1b;
    bsp_pn298_write_reg(W_REGISTER + 0x19, demod_cal);
      
    data[0] = 0x60; 
	data[1] = 0x0e;
    bsp_pn298_write_buf(W_REGISTER + 0x1e, data, 2);

    /* PN298晶振时钟输出到PAD 
     * 寄存器RF_CAL:0x1e
     * [15]:CLK_OUTPAD复用:
     *      0:测试PAD; 1:时钟输出PAD
     * [14]:晶振时钟输出到PAD模块的buf使能:
     *      0:关闭；1:打开
     * [13:12]:晶振时钟输出到PAD频率选择：
     *       00：16M
     *       01：8M
     *       10：4M
     *       11：2M
     */
    data[0] = 0x60;
    data[1] = 0xfe; //0xce:16Mhz
    bsp_pn298_write_buf(W_REGISTER + 0x1e, data, 2);

    /* Feature, 直接64字节模式, 不使用32字节模式 */
    bsp_pn298_write_reg(W_REGISTER + 0x1d, 0x38);
    
    /* noIRQ_noWORK使能 */
    bsp_pn298_write_reg(W_REGISTER + 0x1c, 0xc0);
}


/*
 * @description	: pn298初始化
 * @param		: *addr
				  addr_len
                  chn
				  payload
				  power
				  mode
 * @return		: 无
 */
void __bsp_pn298_init(uint8_t* addr, uint8_t addr_len, uint8_t chn, uint8_t payload, uint16_t power, uint8_t mode)
{
	uint8_t data[2];
	uint8_t f1_data[2];
	uint8_t temp;
	
	/* 设置通信接口为spi */
	data[1] = 0x08;
    data[0] = 0x4f;
	bsp_pn298_write_buf(REG1_3V_WRITE, data, 2);
	
	/* 1.8v 逻辑复位 */
	f1_data[0] = 0x00;
	f1_data[1] = 0x01;
    bsp_pn298_write_reg(REG0_3V_WRITE, f1_data[0]);
	delay_ms(2);
    bsp_pn298_write_reg(REG0_3V_WRITE, f1_data[1]);
	
	/* 1.8v 寄存器初始化 */
	__bsp_pn298_1v8_reg_init();
	
	/* 打开ce */
	bsp_pn298_ce_enable();
	
	/* 设置地址长度 */
	temp = bsp_pn298_read_reg(0x02);
	temp &= ~(0x03 << 6);
	temp |= ((addr_len - 2) << 6);
	bsp_pn298_write_reg(W_REGISTER + 0x02, temp);
	/* 设置模式 */
	bsp_pn298_set_mode(mode);
	/* 设置tx通信地址 */
	bsp_pn298_set_addr(TX_ADDR, addr, addr_len);
    /* 设置rx通信地址 */
	bsp_pn298_set_addr(RX_ADDR_P0, addr, addr_len);
	/* 设置发射功率 */
	bsp_pn298_set_power(power);
	/* 设置通信信道 */
	bsp_pn298_set_channel(chn);
	/* 设置rx payload长度 */
	bsp_pn298_set_rx_payload_len(payload);
	
	
	if((uint8_t)mode  == 0x01){
		bsp_pn298_write_reg(ACTIVATE, 0x73);
		bsp_pn298_write_reg(W_REGISTER + FEATURE, 0x3A);
		bsp_pn298_write_reg(W_REGISTER + EN_AA, 0x01);
	}
}
/*
 * @description	: pn298初始化，并设置为rx模式
 * @param		: 无
 * @return		: 无
 */
void bsp_pn298_init(void)
{
	uint8_t addr[] = __PN298_INIT_ADDR;
    __bsp_pn298_init(addr,__PN298_INIT_ADDR_LEN,__PN298_INIT_CHN,__PN298_PAYLOAD,__PN298_POWER, __PN298_MODE);
    bsp_pn298_rx_mode();
}

/**
  * @brief  单载波
  * @param  None
  * @retval None
  */
void pn298_carrier(void)
{
    uint8_t dat[3] = {0x0b, 0xdf, 0x02};
    
    bsp_pn298_write_reg(W_REGISTER + 0x1C, 0x40);
    bsp_pn298_write_reg(W_REGISTER + 0x00, 0x06);
    bsp_pn298_write_reg(W_REGISTER + 0x19, 0xaf);
    bsp_pn298_write_buf(W_REGISTER + 0x1B, dat, 3);

    bsp_pn298_ce_disable();
}


/**
  * @brief  深度睡眠, 进入后所有配置丢失, 通过重新初始化唤醒
  * @param  None
  * @retval None
  */
void pn298_deep_sleep(void)
{
    bsp_pn298_ce_disable();
    bsp_pn298_write_reg(0xF3, 0x40);
}