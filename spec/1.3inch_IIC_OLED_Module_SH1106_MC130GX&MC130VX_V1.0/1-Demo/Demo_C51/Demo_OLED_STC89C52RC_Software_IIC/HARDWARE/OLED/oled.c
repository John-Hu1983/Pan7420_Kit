//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//测试硬件：单片机STC89C52RC,晶振12M  单片机工作电压3.3V或5V
//QDtech-OLED液晶驱动 for C51
//xiao冯@ShenZhen QDtech co.,LTD
//公司网站:www.qdtft.com
//淘宝网站：http://qdtech.taobao.com
//wiki技术网站：http://www.lcdwiki.com
//我司提供技术支持，任何技术问题欢迎随时交流学习
//固话(传真) :+86 0755-23594567 
//手机:15989313508（冯工） 
//邮箱:lcdwiki01@gmail.com    support@lcdwiki.com    goodtft@163.com
//技术支持QQ:3002773612  3002778157
//技术交流QQ群:324828016
//创建日期:2018/9/14
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 深圳市全动电子技术有限公司 2018-2028
//All rights reserved
/****************************************************************************************************
//=========================================电源接线================================================//
// OLED模块                C51单片机
//   VCC         接       DC 5V/3.3V      //OLED屏电源正
//   GND         接          GND          //OLED屏电源地
//=======================================液晶屏数据线接线==========================================//
//本模块默认数据总线类型为IIC
// OLED模块                C51单片机
//   SDA        接           P15          //OLED屏SPI写信号
//=======================================液晶屏控制线接线==========================================//
// OLED模块                C51单片机
//   SCL        接           P17          //OLED屏SPI时钟信号
//=========================================触摸屏接线=========================================//
//本模块不带触摸功能，所以不需要触摸屏接线
****************************************************************************************************/	
/***************************************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, QD electronic SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
****************************************************************************************************/	
#include "oled.h"
#include "stdlib.h"
#include "string.h" 	 
#include "iic.h"

//OLED显存总共分为8页
//每页8行，一行128个像素点
//OLED的显存
//存放格式如下.
//[0]0 1 2 3 ... 127 (0~7)行	   
//[1]0 1 2 3 ... 127 (8~15)行	
//[2]0 1 2 3 ... 127 (16~23)行	
//[3]0 1 2 3 ... 127 (24~31)行	
//[4]0 1 2 3 ... 127 (32~39)行	
//[5]0 1 2 3 ... 127 (40~47)行	
//[6]0 1 2 3 ... 127 (48~55)行	
//[7]0 1 2 3 ... 127 (56~63)行			   

/*******************************************************************
 * @name       :void OLED_WR_Byte(unsigned dat,unsigned cmd)
 * @date       :2018-08-27
 * @function   :Write a byte of content to the OLED screen
 * @parameters :dat:Content to be written
                cmd:0-write command
								    1-write data
 * @retvalue   :None
********************************************************************/
void OLED_WR_Byte(unsigned dat,unsigned cmd)
{
	if(cmd)
	{
		Write_IIC_Data(dat);
	}
	else
	{
		Write_IIC_Command(dat);
	}
}

/*******************************************************************
 * @name       :void OLED_Set_Pos(unsigned char x, unsigned char y) 
 * @date       :2018-08-27
 * @function   :Set coordinates in the OLED screen
 * @parameters :x:x coordinates
                y:y coordinates
 * @retvalue   :None
********************************************************************/
void OLED_Set_Pos(unsigned char x, unsigned char y) 
{
 	OLED_WR_Byte(YLevel+y,OLED_CMD);
	OLED_WR_Byte((((x+2)&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte(((x+2)&0x0f),OLED_CMD); 
}  

/*******************************************************************
 * @name       :void OLED_Display_On(void) 
 * @date       :2018-08-27
 * @function   :Turn on OLED display
 * @parameters :None
 * @retvalue   :None
********************************************************************/ 	  
/*
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}*/

/*******************************************************************
 * @name       :void OLED_Display_Off(void)
 * @date       :2018-08-27
 * @function   :Turn off OLED display
 * @parameters :None
 * @retvalue   :None
********************************************************************/    
/*
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}*/
	   			
/*******************************************************************
 * @name       :void OLED_Clear(unsigned dat)  
 * @date       :2018-08-27
 * @function   :clear OLED screen
 * @parameters :dat:0-Display full black
                    1-Display full white
 * @retvalue   :None
********************************************************************/ 
void OLED_Clear(unsigned dat)  
{ 
	u8 i,n,color;	
	if(dat)
	{
		color = 0xff;
	}
	else
	{
		color = 0;
	}
	for(i=0;i<PAGE_SIZE;i++)  
	{  
		OLED_WR_Byte(YLevel+i,OLED_CMD);    //设置页地址（0~7）
		OLED_WR_Byte(XLevelL,OLED_CMD);      //设置显示位置—列低地址
		OLED_WR_Byte(XLevelH,OLED_CMD);      //设置显示位置—列高地址   
		for(n=0;n<WIDTH;n++)
		{
			OLED_WR_Byte(color,OLED_DATA); 
		}
	}   //更新显示
}

/*******************************************************************
 * @name       :void OLED_Init(void)
 * @date       :2018-08-27
 * @function   :initialise OLED SH1106 control IC
 * @parameters :None
 * @retvalue   :None
********************************************************************/ 				    
void OLED_Init(void)
{
/**************初始化SH1106*****************/	
	OLED_WR_Byte(0xAE,OLED_CMD);    /*display off*/       
	OLED_WR_Byte(0x02,OLED_CMD);    /*set lower column address*/       
	OLED_WR_Byte(0x10,OLED_CMD);    /*set higher column address*/     
	OLED_WR_Byte(0x40,OLED_CMD);    /*set display start line*/     
	OLED_WR_Byte(0xB0,OLED_CMD);    /*set page address*/     
	OLED_WR_Byte(0x81,OLED_CMD);    /*contract control*/
	OLED_WR_Byte(0xFF,OLED_CMD);    /*128*/     
	OLED_WR_Byte(0xA1,OLED_CMD);    /*set segment remap*/    
	OLED_WR_Byte(0xA6,OLED_CMD);    /*normal / reverse*/     
	OLED_WR_Byte(0xA8,OLED_CMD);    /*multiplex ratio*/
	OLED_WR_Byte(0x3F,OLED_CMD);    /*duty = 1/64*/     
	OLED_WR_Byte(0xAD,OLED_CMD);    /*set charge pump enable*/
	OLED_WR_Byte(0x8B,OLED_CMD);     /*    0x8B    内供VCC   */     
	OLED_WR_Byte(0x32,OLED_CMD);    /*0X30---0X33  set VPP   8V */     
	OLED_WR_Byte(0xC8,OLED_CMD);    /*Com scan direction*/     
	OLED_WR_Byte(0xD3,OLED_CMD);    /*set display offset*/
	OLED_WR_Byte(0x00,OLED_CMD);   /*   0x20  */     
	OLED_WR_Byte(0xD5,OLED_CMD);    /*set osc division*/
	OLED_WR_Byte(0x80,OLED_CMD);         
	OLED_WR_Byte(0xD9,OLED_CMD);    /*set pre-charge period*/
	OLED_WR_Byte(0x1F,OLED_CMD);    /*0x22*/     
	OLED_WR_Byte(0xDA,OLED_CMD);    /*set COM pins*/
	OLED_WR_Byte(0x12,OLED_CMD);     
	OLED_WR_Byte(0xDB,OLED_CMD);    /*set vcomh*/
	OLED_WR_Byte(0x40,OLED_CMD);                
	OLED_WR_Byte(0xAF,OLED_CMD);    /*display ON*/    
}
  





























