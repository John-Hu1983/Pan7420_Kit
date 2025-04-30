#include "../Inc/includes.h"

static u8 OLED_buffer[1024];

/*******************************************************************
 * @name       :void OLED_WR_Byte(unsigned dat,unsigned cmd)
 * @date       :2018-08-27
 * @function   :Write a byte of content to the OLED screen
 * @parameters :dat:Content to be written
								cmd:0-write command
										1-write data
 * @retvalue   :None
********************************************************************/
void OLED_WR_Byte(unsigned dat, unsigned cmd)
{
	if (cmd)
	{
		Write_IIC_Data(dat);
	}
	else
	{
		Write_IIC_Command(dat);
	}
}

/*******************************************************************
 * @name       :void OLED_Set_Pos(u8 x, u8 y)
 * @date       :2018-08-27
 * @function   :Set coordinates in the OLED screen
 * @parameters :x:x coordinates
								y:y coordinates
 * @retvalue   :None
********************************************************************/
void OLED_Set_Pos(u8 x, u8 y)
{
	OLED_WR_Byte(YLevel + y / PAGE_SIZE, OLED_CMD);
	OLED_WR_Byte((((x + 2) & 0xf0) >> 4) | 0x10, OLED_CMD);
	OLED_WR_Byte(((x + 2) & 0x0f), OLED_CMD);
}

/*******************************************************************
 * @name       :void OLED_Display_On(void)
 * @date       :2018-08-27
 * @function   :Turn on OLED display
 * @parameters :None
 * @retvalue   :None
 ********************************************************************/
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D, OLED_CMD); // SET DCDC命令
	OLED_WR_Byte(0X14, OLED_CMD); // DCDC ON
	OLED_WR_Byte(0XAF, OLED_CMD); // DISPLAY ON
}

/*******************************************************************
 * @name       :void OLED_Display_Off(void)
 * @date       :2018-08-27
 * @function   :Turn off OLED display
 * @parameters :None
 * @retvalue   :None
 ********************************************************************/
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D, OLED_CMD); // SET DCDC命令
	OLED_WR_Byte(0X10, OLED_CMD); // DCDC OFF
	OLED_WR_Byte(0XAE, OLED_CMD); // DISPLAY OFF
}

/*******************************************************************
 * @name       :void OLED_Set_Pixel(u8 x, u8 y,u8 color)
 * @date       :2018-08-27
 * @function   :set the value of pixel to RAM
 * @parameters :x:the x coordinates of pixel
								y:the y coordinates of pixel
								color:the color value of the point
											1-white
											0-black
 * @retvalue   :None
********************************************************************/
void OLED_Set_Pixel(u8 x, u8 y, u8 color)
{
	if (color)
	{
		OLED_buffer[(y / PAGE_SIZE) * WIDTH + x] |= (1 << (y % PAGE_SIZE)) & 0xff;
	}
	else
	{
		OLED_buffer[(y / PAGE_SIZE) * WIDTH + x] &= ~((1 << (y % PAGE_SIZE)) & 0xff);
	}
}

/*******************************************************************
 * @name       :void OLED_Display(void)
 * @date       :2018-08-27
 * @function   :Display in OLED screen
 * @parameters :None
 * @retvalue   :None
 ********************************************************************/
void OLED_Display(void)
{
	u8 i, n;
	for (i = 0; i < PAGE_SIZE; i++)
	{
		OLED_WR_Byte(YLevel + i, OLED_CMD); // 设置页地址（0~7）
		OLED_WR_Byte(XLevelL, OLED_CMD);		// 设置显示位置—列低地址
		OLED_WR_Byte(XLevelH, OLED_CMD);		// 设置显示位置—列高地址
		for (n = 0; n < WIDTH; n++)
		{
			OLED_WR_Byte(OLED_buffer[i * WIDTH + n], OLED_DATA);
		}
	} // 更新显示
}

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
	if (dat)
	{
		memset(OLED_buffer, 0xff, sizeof(OLED_buffer));
	}
	else
	{
		memset(OLED_buffer, 0, sizeof(OLED_buffer));
	}
	OLED_Display();
}

/*******************************************************************
 * @name       :void OLED_Init_GPIO(void)
 * @date       :2018-08-27
 * @function   :Reset OLED screen
 * @parameters :None
 * @retvalue   :None
 ********************************************************************/
void OLED_Init_GPIO(void)
{
	APP_ConfigI2cMaster();
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
	memset((void *)OLED_buffer, 0, sizeof(OLED_buffer));
	OLED_Init_GPIO(); // 初始化GPIO
	delay_ms(200);

	/**************初始化SH1106*****************/
	OLED_WR_Byte(0xAE, OLED_CMD); /*display off*/
	OLED_WR_Byte(0x02, OLED_CMD); /*set lower column address*/
	OLED_WR_Byte(0x10, OLED_CMD); /*set higher column address*/
	OLED_WR_Byte(0x40, OLED_CMD); /*set display start line*/
	OLED_WR_Byte(0xB0, OLED_CMD); /*set page address*/
	OLED_WR_Byte(0x81, OLED_CMD); /*contract control*/
	OLED_WR_Byte(0xFF, OLED_CMD); /*128*/
	OLED_WR_Byte(0xA1, OLED_CMD); /*set segment remap*/
	OLED_WR_Byte(0xA6, OLED_CMD); /*normal / reverse*/
	OLED_WR_Byte(0xA8, OLED_CMD); /*multiplex ratio*/
	OLED_WR_Byte(0x3F, OLED_CMD); /*duty = 1/64*/
	OLED_WR_Byte(0xAD, OLED_CMD); /*set charge pump enable*/
	OLED_WR_Byte(0x8B, OLED_CMD); /*    0x8B    内供VCC   */
	OLED_WR_Byte(0x32, OLED_CMD); /*0X30---0X33  set VPP   8V */
	OLED_WR_Byte(0xC8, OLED_CMD); /*Com scan direction*/
	OLED_WR_Byte(0xD3, OLED_CMD); /*set display offset*/
	OLED_WR_Byte(0x00, OLED_CMD); /*   0x20  */
	OLED_WR_Byte(0xD5, OLED_CMD); /*set osc division*/
	OLED_WR_Byte(0x80, OLED_CMD);
	OLED_WR_Byte(0xD9, OLED_CMD); /*set pre-charge period*/
	OLED_WR_Byte(0x1F, OLED_CMD); /*0x22*/
	OLED_WR_Byte(0xDA, OLED_CMD); /*set COM pins*/
	OLED_WR_Byte(0x12, OLED_CMD);
	OLED_WR_Byte(0xDB, OLED_CMD); /*set vcomh*/
	OLED_WR_Byte(0x40, OLED_CMD);
	OLED_WR_Byte(0xAF, OLED_CMD); /*display ON*/
}
