//////////////////////////////////////////////////////////////////////////////////////////////////////	 
//This program is for learning only,Not to be used for any other purpose 
//without the permission of the author
//Testing Hardware:Raspberry PI 3 Model B and B+,  basic frequency:1GHz
//QDtech-OLED liquid crystal driver for Raspberry PI
//xiaofeng@ShenZhen QDtech co.,LTD
//Company Website:www.qdtft.com
//Taobao Website:http://qdtech.taobao.com
//wiki Technology Website:http://www.lcdwiki.com
//We provide technical support,Any technical questions are welcome to 
//exchange and study at any time
//Fixed telephone (fax):+86 0755-23594567 
//cell-phone:15989313508(Mr Feng)
//E-mail:lcdwiki01@gmail.com    support@lcdwiki.com    goodtft@163.com
//Technical Support QQ:3002773612  3002778157
//Technical Exchange QQ group:324828016
//Date:2018/9/14
//Version:V1.0
//Copyright reserved, piracy must be investigated
//Copyright(C) ShenZhen QDtech co.,LTD 2018-2028
//All rights reserved
/****************************************************************************************************
//This program uses the wiringPi gpio library,
//so the pin definition using wiringPi coding
//=====================================power supply wiring===========================================//
// OLED Module                  Raspberry PI    
//    VCC        connect         DC 5V/3.3V        //OLED power positive, Physical pin 1,2,4
//    GND        connect            GND            //OLED power ground,Physical pin 6,9,14,20,25,30,34,39
//======================================data line wiring=============================================//
//The default data bus type for this module is IIC
// OLED Module                  Raspberry PI 
//    SDA        connect        3(wiringPi:8)     //OLED iic write signal
//======================================control line wiring==========================================//
// OLED Module                  Raspberry PI 
//    SCL        connect        5(wiringPi:9)     //OLED iic colck signal
//========================================touch screen wiring========================================//
//This module has no touch function,so it don't need touch screen wiring
*****************************************************************************************************/	
/****************************************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, QD electronic SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*****************************************************************************************************/		
#include "iic.h"

uint32_t iic_fd;


uint32_t IIC_init(void)
{
	uint32_t fd;
	fd = wiringPiI2CSetup (IIC_SLAVE_ADDR);
	return fd;
}

/*****************************************************************************
 * @name       :void IIC_WriteCmd(uint8_t I2C_Command)
 * @date       :2018-09-14 
 * @function   :write a byte of command with iic bus
 * @parameters :I2C_Command:command to be writen
 * @retvalue   :None
******************************************************************************/
void IIC_WriteCmd(uint8_t I2C_Command)
{
	wiringPiI2CWriteReg8(iic_fd, IIC_COMMAND, I2C_Command);
}

/*****************************************************************************
 * @name       :void IIC_WriteDat(uint8_t I2C_Data)
 * @date       :2018-09-14 
 * @function   :write a byte of data with iic bus
 * @parameters :I2C_Data:data to be writen
 * @retvalue   :None
******************************************************************************/
void IIC_WriteDat(uint8_t I2C_Data)
{
	wiringPiI2CWriteReg8(iic_fd, IIC_DATA, I2C_Data);
}

