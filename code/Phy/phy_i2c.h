#ifndef __BSP_I2C_HW_H
#define __BSP_I2C_HW_H

#define I2C_ADDRESS (OLED_DEVICE_ID) // ����\�ӻ���ַ
#define I2C_SPEEDCLOCK (100000)      // ͨѶ�ٶ�100K max 400K
#define I2C_STATE_READY (0)          // ����״̬
#define I2C_STATE_BUSY_TX (1)        // ����״̬
#define I2C_STATE_BUSY_RX (2)        // ����״̬

void APP_ConfigI2cMaster(void);
void APP_MasterTransmit(uint16_t DevAddress, uint8_t *pData, uint16_t Size);
void APP_MasterReceive(uint16_t DevAddress, uint8_t *pData, uint16_t Size);
#endif
