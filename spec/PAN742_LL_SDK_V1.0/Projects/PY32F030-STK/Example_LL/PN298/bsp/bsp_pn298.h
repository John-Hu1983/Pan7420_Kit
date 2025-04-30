/*******************************************************************************
 * @note      Copyright (C) Shanghai Panchip Microelectronics Co., Ltd.
 *            All rights reserved.
 *
 * @file      bsp_pn298.h
 * @brief     pn298 ����
 *
 * @history - V1.0, 2022-02-17, yangyaming, first implementation.
*******************************************************************************/
#ifndef __BSP_PN298_H
#define __BSP_PN298_H

#ifdef __cplusplus
extern "C" {
#endif

#include "bsp.h"


#define	REG0_3V_WRITE					0xf1
#define REG0_3V_READ					0xf2
#define	REG1_3V_WRITE					0xf3
#define REG1_3V_READ					0xf4


//////////////////////////////////////////////////////////////////////////////////////////////////////////
/******************************************** REGISTER ********************************************/
#define	RF_READ_REG						0x00							// �����üĴ���,��5λΪ�Ĵ�����ַ
#define	W_REGISTER						0x20							// д���üĴ���,��5λΪ�Ĵ�����ַ
#define	R_RX_PAYLOAD					0x61							// ��RX��Ч����, 1~32�ֽ�
#define	W_TX_PAYLOAD					0xA0							// дTX��Ч����,1~32�ֽ�
#define	FLUSH_TX						0xE1							// ���TX FIFO�Ĵ���.����ģʽ����
#define	FLUSH_RX						0xE2							// ���RX FIFO�Ĵ���.����ģʽ����
#define	REUSE_TX_PL						0xE3							// ����ʹ����һ������,CEΪ��,���ݰ������Ϸ���
#define	ACTIVATE						0x50							// ACTIVATE
#define	DEACTIVATE						0x50							// DEACTIVATE
#define	R_RX_PL_WID						0x60							// Read width of RX data
#define	W_ACK_PAYLOAD					0xA8							// Data with ACK
#define	W_TX_PAYLOAD_NOACK				0xB0							// TX Payload no ACK Request
#define	CE_ON							0xFD							// CE HIGH
#define	CE_OFF							0xFC							// CE LOW
#define	RST_FSPI						0x53							// RESET
#define	NOP								0xFF							// �ղ���,����������״̬�Ĵ���

//////////////////////////////////////////////////////////////////////////////////////////////////////////
/****************************************** CONTROL   REGISTER ******************************************/
#define	CONFIG							0x00							// ���üĴ�����ַ;bit0:1����ģʽ,0����ģʽ;bit1:��ѡ��;bit2:CRCģʽ;bit3:CRCʹ��;
																		// bit4:�ж�MAX_RT(�ﵽ����ط������ж�)ʹ��;bit5:�ж�TX_DSʹ��;bit6:�ж�RX_DRʹ��
#define	EN_AA							0x01							// ʹ���Զ�Ӧ����  bit0~5,��Ӧͨ��0~5
#define	EN_RXADDR						0x02							// ���յ�ַ����,bit0~5,��Ӧͨ��0~5
#define	SETUP_AW						0x03							// ���õ�ַ���(��������ͨ��):bit1,0:00,3�ֽ�;01,4�ֽ�;02,5�ֽ�;
#define	SETUP_RETR						0x04							// �����Զ��ط�;bit3:0,�Զ��ط�������;bit7:4,�Զ��ط���ʱ 250*x+86us
#define	RF_CH							0x05							// rfͨ��,bit6:0,����ͨ��Ƶ��;
#define	RF_SETUP						0x06							// rf�Ĵ���;bit3:��������(0:1Mbps,1:2Mbps);bit2:1,���书��;bit0:�������Ŵ�������
#define	RF_STATUS						0x07							// ״̬�Ĵ���;bit0:TX FIFO����־;bit3:1,��������ͨ����(���:6);bit4,�ﵽ�����ط�
																		// bit5:���ݷ�������ж�;bit6:���������ж�;
#define	OBSERVE_TX						0x08							// ����״̬�Ĵ���,bit7:4,���ݰ���ʧ������;bit3:0,�ط�������
#define	CD								0x09							// �ز����Ĵ���,bit0,�ز����;
#define	RX_ADDR_P0						0x0A							// ����ͨ��0���յ�ַ,��󳤶�5���ֽ�,���ֽ���ǰ
#define	RX_ADDR_P1						0x0B							// ����ͨ��1���յ�ַ,��󳤶�5���ֽ�,���ֽ���ǰ
#define	RX_ADDR_P2						0x0C							// ����ͨ��2���յ�ַ,����ֽڿ�����,���ֽ�,����ͬRX_ADDR_P1[39:8]���;
#define	RX_ADDR_P3						0x0D							// ����ͨ��3���յ�ַ,����ֽڿ�����,���ֽ�,����ͬRX_ADDR_P1[39:8]���;
#define	RX_ADDR_P4						0x0E							// ����ͨ��4���յ�ַ,����ֽڿ�����,���ֽ�,����ͬRX_ADDR_P1[39:8]���;
#define	RX_ADDR_P5						0x0F							// ����ͨ��5���յ�ַ,����ֽڿ�����,���ֽ�,����ͬRX_ADDR_P1[39:8]���;
#define	TX_ADDR							0x10							// ���͵�ַ(���ֽ���ǰ),ShockBurstTMģʽ��,RX_ADDR_P0��˵�ַ���
#define	RX_PW_P0						0x11							// ��������ͨ��0��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define	RX_PW_P1						0x12							// ��������ͨ��1��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define	RX_PW_P2						0x13							// ��������ͨ��2��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define	RX_PW_P3						0x14							// ��������ͨ��3��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define	RX_PW_P4						0x15							// ��������ͨ��4��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define	RX_PW_P5						0x16							// ��������ͨ��5��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define	FIFO_STATUS						0x17							// FIFO״̬�Ĵ���;bit0,RX FIFO�Ĵ����ձ�־;bit1,RX FIFO����־;bit2,3,����
																		// bit4,TX FIFO�ձ�־;bit5,TX FIFO����־;bit6,1,ѭ��������һ���ݰ�.0,��ѭ��;
#define	DEM_CAL							0x19
#define	RF_CAL2							0x1A
#define	DEM_CAL2						0x1B
#define	DYNPD							0x1C
#define	FEATURE							0x1D
#define	RF_CAL							0x1E
#define	BB_CAL							0x1F


/********************************************* PAN1026 ģʽ *********************************************/
	
#define PN298_XN297L_NORMAL_MODE		0x00							//xn297L��ͨ��ģʽ
#define PN298_XN297L_ENHANCE_MODE		0x01							//xn297L��ǿ��ģʽ
#define PN298_BLE_MODE				0x10							//bleģʽ
#define PN298_NORDIC_NORMAL_MODE		0x20							//nordic��ͨ��ģʽ
#define PN298_NORDIC_ENHANCE_MODE		0x21							//nordic��ǿ��ģʽ
		 
#define PN298_MODE_MSK				0x30



/********************************************* PAN1026 ���� *********************************************/

#define PN298_POWER_P_12				0xc9cf							// 12dBm
#define PN298_POWER_P_11				0x49cf							// 11dBm
#define PN298_POWER_P_10				0x49c7							// 10dBm
#define PN298_POWER_P_7				0xc9da							// 7dBm
#define PN298_POWER_P_6				0xc9c6							// 6dBm
#define PN298_POWER_P_4				0xc9c2							// 4dBm
#define PN298_POWER_P_2				0x49ca							// 2dBm
#define PN298_POWER_P_0				0x49c2							// 0dBm
#define PN298_POWER_N_2				0xc9c1							// -2dBm
#define PN298_POWER_N_10				0x49c1							// -10dBm
		 
#define	PN298_RF_DATA_RATE_1M			0x00
#define	PN298_RF_DATA_RATE_2M			0x40
#define	PN298_RF_DATA_RATE_250K		0xC0

#define DR_1M     PN298_RF_DATA_RATE_1M
#define DR_2M     PN298_RF_DATA_RATE_2M
#define DR_250K   PN298_RF_DATA_RATE_250K

#define	RX_DR_FLAG						0x40							// �����жϱ�־λ
#define	TX_DS_FLAG						0x20							// ��������жϱ�־λ
#define	RX_TX_FLAG						0x60							// ���ͽ�������жϱ�־λ��ack_payload ģʽ��ʹ��
#define	MAX_RT_FLAG						0x10							// �����ش���ʱ�жϱ�־λ

#define	TRANS_ENHANCE_MODE				1								// ������ǿģʽ
#define	TRANS_NORMAL_MODE				2								// ������ͨģʽ

#define	CE_SOFTWARE_MODE				1								// CE���ģʽ
#define	CE_HARDWARE_MODE				2								// CEӲ��ģʽ

//////////////////////////////////////////////////////////////////////////////////////////////////////////
/********************************************* USER  CONFIG *********************************************/
#define	DEFAULT_CHANNEL					CONF_DEFAULE_CNANNEL			// ��ʼƵ��
#define	PAYLOAD_WIDTH					CONF_PAYLOAD_WIDTH				// Payload����
#define	CE_TYPE							CE_SOFTWARE_MODE				// ��ʼ����ΪӲ��ģʽ
#define	TRANSMIT_TYPE					TRANS_ENHANCE_MODE				// ��ʼ����Ϊ��ͨģʽ
#define DATA_RATE						XN297L_RF_DATA_RATE_1M
#define	RF_POWER						(XN297L_RF_POWER_P_10|DATA_RATE)
#define RF_COMMON_ADDR					CONF_DEFAULT_ADDR				// ��ʼ��ַ
#define RF_RETRANS						01
#define EN_ACK_PAYLOAD					1
#define EN_NO_ACK						0

#if(TRANSMIT_TYPE == TRANS_ENHANCE_MODE)
#define	EN_DYNPLOAD						0
#define	EN_ACK_PAYLOAD					1
#define	ACK_PAYLOAD_WIDTH				16
#endif
//////////////////////////////////////////////////////////////////////////////////////////////////////////

void	bsp_pn298_write_reg(uint8_t reg,uint8_t data);												// PN298д�Ĵ�������
uint8_t	bsp_pn298_read_reg(uint8_t reg);															// PN298���Ĵ�������
void	bsp_pn298_write_buf(uint8_t reg,uint8_t *pbuf,uint8_t len);									// PN298д���庯��
void	bsp_pn298_read_buf(uint8_t reg,uint8_t *pbuf,uint8_t len);									// PN298�����庯��
void 	bsp_pn298_ce_enable(void);																	// PN298 CE ʹ��
void 	bsp_pn298_ce_disable(void);																	// PN298_CE ����
uint8_t	bsp_pn298_get_status(void);																	// PN298��ȡ״̬��Ϣ
void	bsp_pn298_clear_status(void);																// PN298���״̬��Ϣ
void 	bsp_pn298_flush_rx_fifo(void);																// PN298���rx����
void 	bsp_pn298_flush_tx_fifo(void);																// PN298���tx����
void	bsp_pn298_clear_fifo(void);																	// PN298�������
void	bsp_pn298_tx_mode(void);																	// PN298����Ϊ����ģʽ
void	bsp_pn298_rx_mode(void);																	// PN298����Ϊ����ģʽ
void 	bsp_pn298_set_power(uint16_t power);														// PN298���÷��书��
void	bsp_pn298_set_channel(uint8_t channel);														// PN298����ͨ���ŵ�
void 	bsp_pn298_set_mode(uint8_t mode);															// PN298����ģʽ
void 	bsp_pn298_set_addr(uint8_t reg, uint8_t *rf_addr_array, uint8_t addr_len);				
void 	bsp_pn298_set_rx_payload_len(uint8_t len);				
void	bsp_pn298_tx_data(uint8_t *data, uint8_t len);												// PN298��������
uint8_t	bsp_pn298_rx_data(uint8_t *data,uint8_t len);												// PN298���պ���
void 	bsp_pn298_byte_bit_decode(uint8_t* buf, uint8_t len);
uint8_t bsp_pn298_rx_data_ack_fix(uint8_t *rx_data, uint8_t *ack_data, uint8_t len, uint8_t ack);
void bsp_pn298_tx_data_ack(uint8_t *data, uint8_t *ack_data, uint8_t len);
		
void	bsp_pn298_init(void);																		// PN298��ʼ��
void pn298_carrier(void);
void pn298_deep_sleep(void);
#ifdef __cplusplus
}
#endif

#endif // __DRV_PAN1026_H
