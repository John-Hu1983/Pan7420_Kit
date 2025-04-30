#ifndef __BSP_I2C_HW_H
#define __BSP_I2C_HW_H

#define I2C_ADDRESS (OLED_DEVICE_ID) // 本机\从机地址
#define I2C_SPEEDCLOCK (100000)      // 通讯速度100K max 400K
#define I2C_STATE_READY (0)          // 就绪状态
#define I2C_STATE_BUSY_TX (1)        // 发送状态
#define I2C_STATE_BUSY_RX (2)        // 接收状态

void APP_ConfigI2cMaster(void);
void APP_MasterTransmit(uint16_t DevAddress, uint8_t *pData, uint16_t Size);
void APP_MasterReceive(uint16_t DevAddress, uint8_t *pData, uint16_t Size);
#endif
