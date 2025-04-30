#include "../Inc/includes.h"

extern __IO uint32_t State;

/*****************************************************************************
 * @name       :void Write_IIC_Command(u8 IIC_Command)
 * @date       :2018-09-13
 * @function   :Write a byte of command to oled screen
 * @parameters :IIC_Command:command to be written
 * @retvalue   :None
 ******************************************************************************/
void Write_IIC_Command(u8 IIC_Command)
{
  u8 aTxBuffer[2];
  aTxBuffer[0] = 0x00;
  aTxBuffer[1] = IIC_Command;
  APP_MasterTransmit(I2C_ADDRESS, (uint8_t *)aTxBuffer, sizeof(aTxBuffer));
  while (State != I2C_STATE_READY)
    ;
}

/*****************************************************************************
 * @name       :void Write_IIC_Data(u8 IIC_Data)
 * @date       :2018-09-13
 * @function   :Write a byte of data to oled screen
 * @parameters :IIC_Data:data to be written
 * @retvalue   :None
 ******************************************************************************/
void Write_IIC_Data(u8 IIC_Data)
{
  u8 aTxBuffer[2];
  aTxBuffer[0] = 0x40;
  aTxBuffer[1] = IIC_Data;
  APP_MasterTransmit(I2C_ADDRESS, (uint8_t *)aTxBuffer, sizeof(aTxBuffer));
  while (State != I2C_STATE_READY)
    ;
}
