/**
  ******************************************************************************
  * @file    main.c
  * @author  MCU Application Team
  * @Version V1.0.0
  * @Date    2022-8-19
  * @brief   main function
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "py32f030xx_ll_Start_Kit.h"
#include "bsp.h"
#include "stdio.h"


#define EXAMPLE_NAME	"PN298 enhanced mode compatible with xn297l"
#define EXAMPLE_DATE	"2022-08-08"
#define DEMO_VER		"1.0.0"

/* Private define ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void APP_ConfigGpio(void);
void GPIO_Config(void);
LL_UTILS_ClkInitTypeDef UTILS_ClkInitStruct;

#define __PN298_TX_PERIOD		5
static uint8_t tx_rx_mode = 0;	//0: 表示PN298工作于接收状态，1：表示PN298工作于发送状态
static uint8_t temp_buff[32];		//数据接收或者发送缓冲区
static uint8_t ack_buff[__PN298_PAYLOAD];
static uint8_t rx_buff[__PN298_PAYLOAD];
/*
 * @description	: 显示该例程的一些信息
 * @param		: 无 
 * @return		: 无
 */
static void printf_logo(void)
{
	printf("*************************************************************\r\n");
	printf("* 例程名称   : %s\r\n", EXAMPLE_NAME);		/* 打印例程名称 */
	printf("* 例程版本   : %s\r\n", DEMO_VER);			/* 打印例程版本 */
	printf("* 发布日期   : %s\r\n", EXAMPLE_DATE);		/* 打印例程日期 */
	printf("* MCU主频    : %dHz\r\n", SystemCoreClock);	/* 打印系统主频 */
  printf("* Copyright www.panchip.com\r\n");
	printf("*************************************************************\r\n");
}

/*
 * @description	: 显示提示信息
 * @param		: 无 
 * @return		: 无
 */
static void printf_help(void)
{
	printf("*************************************************************\r\n");
	printf("操作提示:\r\n");
  printf("串口输入1:Start send data\r\n");
	printf("串口输入2:Stop send data\r\n");
	printf("串口输入3:Send 1000 packet data\r\n");
	printf("串口输入4:Set tx and rx mode\r\n");
	printf("*************************************************************\r\n");
}


/*
 * @description	: 显示内容初始化
 * @param		: 无 
 * @return		: 无
 */
static void disp_init(void)
{
	static uint8_t first_init = 0x00;
	
  printf("*************************************************************\r\n");
	printf("* PN298 Default Config *\r\n");
	
	
	/* 初始化默认为接收模式 */
	if(first_init == 0x00){
		first_init = 0x01;
		bsp_pn298_rx_mode();
      printf("* Mode: rx mode\r\n");        
	}else if(first_init == 0x01){
		if(tx_rx_mode == 1){
			printf("* Mode: tx mode\r\n");  
		}			
	}
    printf("* Channel: 0x%x\r\n",__PN298_INIT_CHN);   
    printf("* Payload: 0x%x\r\n",__PN298_PAYLOAD);
		printf("* Tx Period(ms):%d\r\n",__PN298_TX_PERIOD);
    printf("*************************************************************\r\n");
}

/*******************************************************************************
**功能描述 ：执行函数
**输入参数 ：
**输出参数 ：
*******************************************************************************/
int main(void)
{
		uint8_t key_code;
	uint16_t count = 0;
	uint16_t i;
	
	for(i = 0; i < __PN298_PAYLOAD; i++){
		temp_buff[i] = i;
	}
	
  /* 配置系统时钟 */
  SystemClock_Config();


//	GPIO_Config();
	 /*MCO输出时钟及分频初始化*/
//  LL_RCC_ConfigMCO(LL_RCC_MCO1SOURCE_SYSCLK,LL_RCC_MCO1_DIV_128);	
  
	bsp_init();
	printf_logo();
	disp_init();
	printf_help();
	
	
  while (1)
  {
		if(bsp_check_timer(0)){
//			BSP_LED_Toggle(LED_GREEN);
			bsp_start_timer(0, __PN298_TX_PERIOD);
			/* 发送数据，每次发送32个字节 */
			bsp_pn298_tx_data_ack(temp_buff, ack_buff, __PN298_PAYLOAD);
			printf("Tx cnt:%d\r\n", ++count);
		}
		
		/* 如果PN298工作于接收模式，进入如下程序 */
		if(tx_rx_mode == 0) {
			if(bsp_pn298_rx_data_ack_fix(rx_buff, temp_buff, __PN298_PAYLOAD, 1)){
				printf("Rx cnt:%d\r\n", ++count);
			}	
		}

			if(SUCCESS == bsp_uart_get_byte(&key_code))
			{
			switch(key_code){
				case 0x31:{
					printf("Start send data\r\n");
					bsp_start_timer(0, __PN298_TX_PERIOD);
					break;
				}
				case 0x32:{
					printf("input 2\r\n");
					printf("Stop send data\r\n");
					count = 0;
					bsp_stop_timer(0);
					break;
				}
				case 0x33:{
					printf("input 3\r\n");
					/* 发送1000包 */
                    printf("Send 1000 packet data\r\n");
					count = 0;
					for(i = 0; i < 1000; i++){
						bsp_pn298_tx_data_ack(temp_buff, ack_buff, __PN298_PAYLOAD);
                        printf("Tx cnt:%d\r\n", ++count);
						delay_ms(__PN298_TX_PERIOD);
					}
					break;
				}
				case 0x34:{
					printf("input 4\r\n");
					if(tx_rx_mode == 0){
						count = 0;
						tx_rx_mode = 1;
						bsp_pn298_tx_mode(); 
                        printf("Current mode: tx mode\r\n");
					}else{
						count = 0;
						tx_rx_mode = 0;
						bsp_pn298_rx_mode(); 
                        printf("Current mode: rx mode\r\n");
					}
					break;
				}
			}
		}
  }
}


/*******************************************************************************
**功能描述 ：系统时钟配置函数
**输入参数 ：
**输出参数 ：
*******************************************************************************/
void SystemClock_Config(void)
{
  /* HSI使能及初始化 */
  LL_RCC_HSI_Enable();
  LL_RCC_HSI_SetCalibFreq(LL_RCC_HSICALIBRATION_24MHz);
  while(LL_RCC_HSI_IsReady() != 1)
  {
		
  }

  LL_PLL_ConfigSystemClock_HSI(&UTILS_ClkInitStruct);
  
  /* 设置AHB分频*/
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);

  /* 配置HSISYS为系统时钟及初始化 */
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {
  }

  /*设置APB1分频及初始化*/
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  LL_SetSystemCoreClock(48000000);
	
  LL_Init1msTick(48000000);

}

/*******************************************************************************
**功能描述 ：配置PA08为MCO复用功能
**输入参数 ：
**输出参数 ：
*******************************************************************************/
void GPIO_Config(void)
{
  /* GPIOA时钟使能*/
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);
  
  /* 配置PA08为复用模式，并复用为MCO输出引脚*/
  LL_GPIO_InitTypeDef GPIO_InitStruct;  
  /* 选择8号引脚*/
  GPIO_InitStruct.Pin = LL_GPIO_PIN_8; 
  /* 配置为复用模式*/	
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE; 
  /* 选择复用为AF5功能*/	
  GPIO_InitStruct.Alternate = LL_GPIO_AF5_MCO;     
  /* 输出速度选择*/	
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;   
  /* 输出模式选择*/
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  /* 无上下拉*/	
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;                
  
  /* 初始化GPIOA*/
  LL_GPIO_Init(GPIOA,&GPIO_InitStruct);
    
}
/*******************************************************************************
**功能描述 ：错误执行函数
**输入参数 ：
**输出参数 ：
*******************************************************************************/
void Error_Handler(void)
{
  while(1)
  {
  }
}

/*******************************************************************************
**功能描述 ：配置LED引脚 (PA11)
**输入参数 ：
**输出参数 ：
*******************************************************************************/
static void APP_ConfigGpio(void)
{
  /* 使能时钟 */
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);

  /* 将PA11引脚配置为输出 */
  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_4, LL_GPIO_MODE_OUTPUT);
  /* 默认（复位后）为推挽输出 */
  // LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_11, LL_GPIO_OUTPUT_PUSHPULL);
  /* 默认（复位后）是非常低的输出速度 */
  // LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_11, LL_GPIO_SPEED_FREQ_LOW);
  /* 默认（复位后）为无上拉 */
  // LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_11, LL_GPIO_PULL_NO);
}

#ifdef  USE_FULL_ASSERT
/*******************************************************************************
**功能描述 ：输出产生断言错误的源文件名及行号
**输入参数 ：file：源文件名指针
**输入参数 ：line：发生断言错误的行号
**输出参数 ：
*******************************************************************************/
void assert_failed(uint8_t *file, uint32_t line)
{
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT Puya *****END OF FILE****/
