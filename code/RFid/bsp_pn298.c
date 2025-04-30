#include "../Inc/includes.h"

/*
 * @description	: ��PN298�Ĵ���д��һ������
 * @param		: reg
				��data
 * @return		: ��
 */
void bsp_pn298_write_reg(uint8_t reg, uint8_t data)
{
	__rfspi_pan742_write_cs(0);
	rfspi_pan742_write_byte(reg);
	rfspi_pan742_write_byte(data);
	__rfspi_pan742_write_cs(1);
}
/*
 * @description	: ��PN298�Ĵ�����ȡһ������
 * @param		: reg
 * @return		: ��ȡ��������
 */
uint8_t bsp_pn298_read_reg(uint8_t reg)
{
	uint8_t temp;
	__rfspi_pan742_write_cs(0);
	rfspi_pan742_write_byte(reg);
	temp = rfspi_pan742_read_byte();
	__rfspi_pan742_write_cs(1);
	return temp;
}
/*
 * @description	: ��PN298�Ĵ�����ȡ�������
 * @param		: reg
				��*pbuf
				��length
 * @return		: ��
 */
void bsp_pn298_write_buf(uint8_t reg, uint8_t *pbuf, uint8_t len)
{
	uint8_t i;
	__rfspi_pan742_write_cs(0);
	rfspi_pan742_write_byte(reg);

	for (i = 0; i < len; i++)
	{
		rfspi_pan742_write_byte(pbuf[i]);
	}
	__rfspi_pan742_write_cs(1);
}
/*
 * @description	: ��PN298�Ĵ�����ȡ�������
 * @param		: reg
				��*pbuf
				��length
 * @return		: ��
 */
void bsp_pn298_read_buf(uint8_t reg, uint8_t *pbuf, uint8_t len)
{
	uint8_t i;
	__rfspi_pan742_write_cs(0);
	rfspi_pan742_write_byte(reg);
	for (i = 0; i < len; i++)
	{
		pbuf[i] = rfspi_pan742_read_byte();
	}
	__rfspi_pan742_write_cs(1);
}
/*
 * @description	: ��ce
 * @param		: ��
 * @return		: ��
 */

void bsp_pn298_ce_enable(void)
{
	bsp_pn298_write_reg(CE_ON, 0x00);
}
/*
 * @description	: �ر�ce
 * @param		: ��
 * @return		: ��
 */
void bsp_pn298_ce_disable(void)
{
	bsp_pn298_write_reg(CE_OFF, 0x00);
}
/*
 * @description	: ��ȡPN298��״̬
 * @param		: ��
 * @return		: ��
 */
uint8_t bsp_pan1026_get_status(void)
{
	return bsp_pn298_read_reg(0x07);
}

/*
 * @description	: ���״̬
 * @param		: ��
 * @return		: ��
 */
void bsp_pn298_clear_status(void)
{
	bsp_pn298_write_reg(W_REGISTER + 0x07, 0x70);
}
/*
 * @description	: ���rx fifo
 * @param		: ��
 * @return		: ��
 */
void bsp_pn298_flush_rx_fifo(void)
{
	bsp_pn298_write_reg(FLUSH_RX, 0x00);
}
/*
 * @description	: ��ce
 * @param		: ��
 * @return		: ��
 */
void bsp_pn298_flush_tx_fifo(void)
{
	bsp_pn298_write_reg(FLUSH_TX, 0x00);
}
/*
 * @description	: ���rx��tx fifo
 * @param		: ��
 * @return		: ��
 */
void bsp_pn298_clear_fifo(void)
{
	bsp_pn298_write_reg(FLUSH_TX, 0x00);
	bsp_pn298_write_reg(FLUSH_RX, 0x00);
}

/*
 * @description	: ����txģʽ
 * @param		: ��
 * @return		: ��
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
 * @description	: ����Ϊrxģʽ
 * @param		: ��
 * @return		: ��
 */
void bsp_pn298_rx_mode(void)
{
	uint8_t temp;
	temp = bsp_pn298_read_reg(0x00);
	temp |= 0x01;
	bsp_pn298_write_reg(W_REGISTER + 0x00, temp);
}
/*
 * @description	: ���÷��书��
 * @param		: power
 * @return		: ��
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
 * @description	: �����ŵ�
 * @param		: chn
 * @return		: ��
 */
void bsp_pn298_set_channel(uint8_t chn)
{
	chn += 64;
	bsp_pn298_write_reg(W_REGISTER + 0x05, chn);
}
/*
 * @description	: ����ģʽ
 * @param		: mode
 * @return		: ��
 */
void bsp_pn298_set_mode(uint8_t mode)
{
	uint8_t data[5];
	uint8_t temp;
	/* nordicģʽ�ر����� */
	temp = bsp_pn298_read_reg(0x19);
	if ((mode == PN298_NORDIC_NORMAL_MODE) || (mode == PN298_NORDIC_ENHANCE_MODE))
	{
		temp &= ~(0x01);
	}
	else
	{
		temp |= 0x01;
	}

	bsp_pn298_write_reg(W_REGISTER + 0x19, temp);

	/* nordic��ǿ���е�����ʱ������ */
	if (mode == PN298_NORDIC_ENHANCE_MODE)
	{
		data[0] = 0xcc;
		data[1] = 0x41;
		data[2] = 0x04;
	}
	else
	{
		data[0] = 0xd3;
		data[1] = 0x59;
		data[2] = 0x0c;
	}
	data[3] = 0x50;
	data[4] = 0x9c;
	bsp_pn298_write_buf(W_REGISTER + 0x1f, data, 5);

	/* ����ģʽ */
	temp = bsp_pn298_read_reg(0x00);
	temp &= ~(0x03 << 6);
	temp |= (((uint8_t)mode & PN298_MODE_MSK) << 2);
	bsp_pn298_write_reg(W_REGISTER + 0x00, temp);

	/* ��ǿ�������ǿ���в�ͬ������ */
	temp = bsp_pn298_read_reg(0x01);
	if (((uint8_t)mode & 0x01) == 0)
	{
		temp &= ~(0x01);
		bsp_pn298_write_reg(W_REGISTER + 0x04, 0x00);
	}
	else
	{
		temp |= 0x01;
		bsp_pn298_write_reg(W_REGISTER + 0x04, 0x01); /* ��ʧ�ܲ��Զ��ش� */
	}
	bsp_pn298_write_reg(W_REGISTER + 0x01, temp);
}

/*
 * @description	: ����ͨ�ŵ�ַ
 * @param		: reg
					*rf_addr_array
					addr_len
 * @return		: ��
 */
void bsp_pn298_set_addr(uint8_t reg, uint8_t *rf_addr_array, uint8_t addr_len)
{
	bsp_pn298_write_buf(W_REGISTER + reg, rf_addr_array, addr_len);
}
/*
 * @description	: ����payload����
 * @param		: len
 * @return		: ��
 */
void bsp_pn298_set_rx_payload_len(uint8_t len)
{
	if (len <= 64)
	{
		bsp_pn298_write_reg(W_REGISTER + 0x11, len);
	}
}
/*
 * @description	: ��λ����ֽ���
 * @param		: *buf
					len
 * @return		: ��
 */
void bsp_pn298_byte_bit_decode(uint8_t *buf, uint8_t len)
{
	uint8_t data1;
	uint8_t data2;
	uint8_t i;
	uint8_t j;
	uint8_t *p1;
	uint8_t *p2;

	i = len >> 1;
	p1 = buf;
	p2 = buf + len - 1;

	if ((len & 1) != 0)
	{
		/* len����, �м�һ���ֽ�����while�ﲻ�ᴦ��, �ȶ��⴦�� */
		if (len > 1)
		{
			buf = buf + i;
		}
		data1 = *buf;

		*buf = 0;
		for (j = 0; j < 7; j++)
		{
			*buf += ((data1 >> j) & 0x01);
			*buf = *buf << 1;
		}
		*buf += (data1 >> 7);
	}

	while (i--)
	{
		data1 = *p1;
		data2 = *p2;
		*p1 = *p2 = 0;
		for (j = 0; j < 7; j++)
		{
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
 * @description	: ��������
 * @param		: data
					len
 * @return		: ��
 */
void bsp_pn298_tx_data(uint8_t *data, uint8_t len)
{
	bsp_pn298_write_buf(W_TX_PAYLOAD, data, len);
	while (!((bsp_pn298_read_reg(RF_STATUS) & TX_DS_FLAG) == TX_DS_FLAG))
		;
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
	while (status == 0x00)
		;

	if (status == RX_TX_FLAG)
	{
		bsp_pn298_read_buf(R_RX_PAYLOAD, ack_data, len); // ��ȡACK����
		printf("Tx cnt:%d\r\n", ++cnt);
	}
	bsp_pn298_flush_rx_fifo();
	bsp_pn298_flush_tx_fifo();
	bsp_pn298_clear_status();
}
/*
 * @description	: ��������
 * @param		: *data
					len
 * @return		: 0 �� 1
 */
uint8_t bsp_pn298_rx_data(uint8_t *data, uint8_t len)
{
	if (!((bsp_pn298_read_reg(RF_STATUS) & RX_DR_FLAG) == RX_DR_FLAG))
	{
		return 0;
	}
	bsp_pn298_read_buf(R_RX_PAYLOAD, data, len);
	bsp_pn298_byte_bit_decode(data, len);
	bsp_pn298_flush_rx_fifo();
	bsp_pn298_clear_status();
	return 1;
}
/*
 * @description	: �������ݣ�����ack
 * @param		: *rx_data
					*ack_data
					len
					ack
 * @return		: 0 �� 1
 */

uint8_t bsp_pn298_rx_data_ack_fix(uint8_t *rx_data, uint8_t *ack_data, uint8_t len, uint8_t ack)
{
	if ((bsp_pn298_read_reg(RF_STATUS) & RX_DR_FLAG) == RX_DR_FLAG)
	{
		if (ack)
		{
			bsp_pn298_write_reg(FLUSH_TX, 0);									 // ������ͻ���
			bsp_pn298_write_buf(W_ACK_PAYLOAD, ack_data, len); // ����ACK����
		}
		// delay_us(300);
		bsp_pn298_read_buf(R_RX_PAYLOAD, rx_data, len);		 // ��������
		bsp_pn298_write_reg(FLUSH_RX, 0);									 // ������ջ���
		bsp_pn298_write_reg(W_REGISTER + RF_STATUS, 0x70); // ���״̬
		return 1;
	}
	return 0;
}
/*
 * @description	: 1.8v�Ĵ�����ʼ��
 * @param		: ��
 * @return		: ��
 */
void __bsp_pn298_1v8_reg_init(void)
{
	uint8_t data[5];
	uint8_t demod_cal;

	data[0] = 0x20;
	/* data[1] = 0xd0; data[2] = 0xff; */ /* ������ ���������Ⱥ� */
	data[1] = 0xc0;
	data[2] = 0xfb; /* ����С ���������Ȳ�1dB */
	bsp_pn298_write_buf(W_REGISTER + 0x03, data, 3);

	data[0] = 0x48;
	data[1] = 0x2a;
	data[2] = 0xfd;
	bsp_pn298_write_buf(W_REGISTER + 0x06, data, 3);

	data[0] = 0xfd;
	data[1] = 0x3f;
	bsp_pn298_write_buf(W_REGISTER + 0x0e, data, 2);

	data[0] = 0x8b;
	data[1] = 0xc9; // A�汾��0xc9; B�汾��0x09
	data[2] = 0x5f;
	bsp_pn298_write_buf(W_REGISTER + 0x18, data, 3);

	demod_cal = 0x1b;
	bsp_pn298_write_reg(W_REGISTER + 0x19, demod_cal);

	data[0] = 0x60;
	data[1] = 0x0e;
	bsp_pn298_write_buf(W_REGISTER + 0x1e, data, 2);

	/* PN298����ʱ�������PAD
	 * �Ĵ���RF_CAL:0x1e
	 * [15]:CLK_OUTPAD����:
	 *      0:����PAD; 1:ʱ�����PAD
	 * [14]:����ʱ�������PADģ���bufʹ��:
	 *      0:�رգ�1:��
	 * [13:12]:����ʱ�������PADƵ��ѡ��
	 *       00��16M
	 *       01��8M
	 *       10��4M
	 *       11��2M
	 */
	data[0] = 0x60;
	data[1] = 0xfe; // 0xce:16Mhz
	bsp_pn298_write_buf(W_REGISTER + 0x1e, data, 2);

	/* Feature, ֱ��64�ֽ�ģʽ, ��ʹ��32�ֽ�ģʽ */
	bsp_pn298_write_reg(W_REGISTER + 0x1d, 0x38);

	/* noIRQ_noWORKʹ�� */
	bsp_pn298_write_reg(W_REGISTER + 0x1c, 0xc0);
}

/*
 * @description	: pn298��ʼ��
 * @param		: *addr
					addr_len
									chn
					payload
					power
					mode
 * @return		: ��
 */
void __bsp_pn298_init(uint8_t *addr, uint8_t addr_len, uint8_t chn, uint8_t payload, uint16_t power, uint8_t mode)
{
	uint8_t data[2];
	uint8_t f1_data[2];
	uint8_t temp;

	/* ����ͨ�Žӿ�Ϊspi */
	data[1] = 0x08;
	data[0] = 0x4f;
	bsp_pn298_write_buf(REG1_3V_WRITE, data, 2);

	/* 1.8v �߼���λ */
	f1_data[0] = 0x00;
	f1_data[1] = 0x01;
	bsp_pn298_write_reg(REG0_3V_WRITE, f1_data[0]);
	delay_ms(2);
	bsp_pn298_write_reg(REG0_3V_WRITE, f1_data[1]);

	/* 1.8v �Ĵ�����ʼ�� */
	__bsp_pn298_1v8_reg_init();

	/* ��ce */
	bsp_pn298_ce_enable();

	/* ���õ�ַ���� */
	temp = bsp_pn298_read_reg(0x02);
	temp &= ~(0x03 << 6);
	temp |= ((addr_len - 2) << 6);
	bsp_pn298_write_reg(W_REGISTER + 0x02, temp);
	/* ����ģʽ */
	bsp_pn298_set_mode(mode);
	/* ����txͨ�ŵ�ַ */
	bsp_pn298_set_addr(TX_ADDR, addr, addr_len);
	/* ����rxͨ�ŵ�ַ */
	bsp_pn298_set_addr(RX_ADDR_P0, addr, addr_len);
	/* ���÷��书�� */
	bsp_pn298_set_power(power);
	/* ����ͨ���ŵ� */
	bsp_pn298_set_channel(chn);
	/* ����rx payload���� */
	bsp_pn298_set_rx_payload_len(payload);

	if ((uint8_t)mode == 0x01)
	{
		bsp_pn298_write_reg(ACTIVATE, 0x73);
		bsp_pn298_write_reg(W_REGISTER + FEATURE, 0x3A);
		bsp_pn298_write_reg(W_REGISTER + EN_AA, 0x01);
	}
}
/*
 * @description	: pn298��ʼ����������Ϊrxģʽ
 * @param		: ��
 * @return		: ��
 */
void bsp_pn298_init(void)
{
	uint8_t addr[] = __PN298_INIT_ADDR;
	__bsp_pn298_init(addr, __PN298_INIT_ADDR_LEN, __PN298_INIT_CHN, __PN298_PAYLOAD, __PN298_POWER, __PN298_MODE);
	bsp_pn298_rx_mode();
}

/**
 * @brief  ���ز�
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
 * @brief  ���˯��, ������������ö�ʧ, ͨ�����³�ʼ������
 * @param  None
 * @retval None
 */
void pn298_deep_sleep(void)
{
	bsp_pn298_ce_disable();
	bsp_pn298_write_reg(0xF3, 0x40);
}