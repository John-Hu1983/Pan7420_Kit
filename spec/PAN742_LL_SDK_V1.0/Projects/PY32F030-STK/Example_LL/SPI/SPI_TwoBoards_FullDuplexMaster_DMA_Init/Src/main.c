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

/* Private define ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* 发送缓冲区定义 */
uint8_t aTxBuffer[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
uint8_t ubNbDataToTransmit = sizeof(aTxBuffer);

/* 接收缓冲区定义 */
uint8_t aRxBuffer[sizeof(aTxBuffer)] = {0};
uint8_t ubNbDataToReceive = sizeof(aTxBuffer);

/* 收发数据完成标志 */
uint8_t txRxDataComplteFlag = RESET;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void APP_ConfigSPI(void);
static void APP_SpiTransmitReceive_DMA(uint8_t *pTxData, uint8_t *pRxData, uint16_t Size);
static void APP_WaitAndCheckEndOfTransfer(void);
static uint8_t APP_Buffercmp8(uint8_t* pBuffer1, uint8_t* pBuffer2, uint8_t BufferLength);
static void APP_LedBlinking(void);

/*******************************************************************************
**功能描述 ：执行函数
**输入参数 ：
**输出参数 ：
*******************************************************************************/
int main(void)
{
  /* 配置系统时钟 */
  SystemClock_Config();
  
  /* 初始化LED */
  BSP_LED_Init(LED_GREEN);

  /* 初始化按键BUTTON */
  BSP_PB_Init(BUTTON_KEY,BUTTON_MODE_GPIO);

 /* 初始化SPI */
  APP_ConfigSPI();

  /*等待按键按下*/
  while(BSP_PB_GetState(BUTTON_KEY) == 1);
  
  /* SPI收发数据 */
  APP_SpiTransmitReceive_DMA(aTxBuffer, aRxBuffer, ubNbDataToTransmit);
  
  /* 等待传输结束并检查接收到的数据 */
  APP_WaitAndCheckEndOfTransfer();
  
  while (1)
  {
    
  }
}

/*******************************************************************************
**功能描述 ：系统时钟配置函数
**输入参数 ：
**输出参数 ：
*******************************************************************************/
void SystemClock_Config(void)
{
  /* 使能HSI */
  LL_RCC_HSI_Enable();
  while(LL_RCC_HSI_IsReady() != 1)
  {
  }

  /* 设置 AHB 分频*/
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);

  /* 配置HSISYS作为系统时钟源 */
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSISYS);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSISYS)
  {
  }

  /* 设置 APB1 分频*/
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_Init1msTick(8000000);

  /* 更新系统时钟全局变量SystemCoreClock(也可以通过调用SystemCoreClockUpdate函数更新) */
  LL_SetSystemCoreClock(8000000);
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
**功能描述 ：SPI1配置函数
**输入参数 ：
**输出参数 ：
*******************************************************************************/
static void APP_ConfigSPI(void)
{
  LL_SPI_InitTypeDef SPI_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* 使能时钟 */
  LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_SPI1);

  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOB);
  
  LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_SYSCFG);
  
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);
  
  /**SPI1 引脚配置
  PB3   ------> SPI1_SCK
  PB4   ------> SPI1_MISO
  PB5   ------> SPI1_MOSI
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_3;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_0;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LL_GPIO_PIN_4;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_0;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LL_GPIO_PIN_5;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_0;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* DMA配置 */
  /* 配置DMA请求映像 */
  LL_SYSCFG_SetDMARemap_CH1(LL_SYSCFG_DMA_MAP_SPI1_TX);
  LL_SYSCFG_SetDMARemap_CH2(LL_SYSCFG_DMA_MAP_SPI1_RX);
  
  /* DMA通道1初始化 */
  LL_DMA_InitTypeDef DMA_InitStruct;
  DMA_InitStruct.PeriphOrM2MSrcAddress  = 0x00000000U;
  DMA_InitStruct.MemoryOrM2MDstAddress  = 0x00000000U;
  DMA_InitStruct.Direction              = LL_DMA_DIRECTION_MEMORY_TO_PERIPH;
  DMA_InitStruct.Mode                   = LL_DMA_MODE_NORMAL;
  DMA_InitStruct.PeriphOrM2MSrcIncMode  = LL_DMA_PERIPH_NOINCREMENT;
  DMA_InitStruct.MemoryOrM2MDstIncMode  = LL_DMA_MEMORY_INCREMENT;
  DMA_InitStruct.PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_BYTE;
  DMA_InitStruct.MemoryOrM2MDstDataSize = LL_DMA_MDATAALIGN_BYTE;
  DMA_InitStruct.NbData                 = 0x00000000U;
  DMA_InitStruct.Priority               = LL_DMA_PRIORITY_VERYHIGH;
  LL_DMA_Init(DMA1, LL_DMA_CHANNEL_1, &DMA_InitStruct);
  
  /* DMA通道2初始化 */
  DMA_InitStruct.PeriphOrM2MSrcAddress  = 0x00000000U;
  DMA_InitStruct.MemoryOrM2MDstAddress  = 0x00000000U;
  DMA_InitStruct.Direction              = LL_DMA_DIRECTION_PERIPH_TO_MEMORY;
  DMA_InitStruct.Mode                   = LL_DMA_MODE_NORMAL;
  DMA_InitStruct.PeriphOrM2MSrcIncMode  = LL_DMA_PERIPH_NOINCREMENT;
  DMA_InitStruct.MemoryOrM2MDstIncMode  = LL_DMA_MEMORY_INCREMENT;
  DMA_InitStruct.PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_BYTE;
  DMA_InitStruct.MemoryOrM2MDstDataSize = LL_DMA_MDATAALIGN_BYTE;
  DMA_InitStruct.NbData                 = 0x00000000U;
  DMA_InitStruct.Priority               = LL_DMA_PRIORITY_LOW;
  LL_DMA_Init(DMA1, LL_DMA_CHANNEL_2, &DMA_InitStruct);
  
  /* 使能DMA的NVIC中断 */
  NVIC_SetPriority(DMA1_Channel1_IRQn, 1);
  NVIC_EnableIRQ(DMA1_Channel1_IRQn);
  
  NVIC_SetPriority(DMA1_Channel2_3_IRQn, 1);
  NVIC_EnableIRQ(DMA1_Channel2_3_IRQn);

  /* SPI1 参数配置*/
  SPI_InitStruct.TransferDirection = LL_SPI_FULL_DUPLEX;
  SPI_InitStruct.Mode = LL_SPI_MODE_MASTER;
  SPI_InitStruct.DataWidth = LL_SPI_DATAWIDTH_8BIT;
  SPI_InitStruct.ClockPolarity = LL_SPI_POLARITY_HIGH;
  SPI_InitStruct.ClockPhase = LL_SPI_PHASE_1EDGE;
  SPI_InitStruct.NSS = LL_SPI_NSS_SOFT;
  SPI_InitStruct.BaudRate = LL_SPI_BAUDRATEPRESCALER_DIV128;
  SPI_InitStruct.BitOrder = LL_SPI_MSB_FIRST;
  LL_SPI_Init(SPI1, &SPI_InitStruct);
}

/*******************************************************************************
**功能描述 ：SPI1收发函数
**输入参数 ：pTxData：发送数据缓冲区；
**           Size：收发数据的大小
**输出参数 ：pRxData：接收数据缓冲区；
*******************************************************************************/
static void APP_SpiTransmitReceive_DMA(uint8_t *pTxData, uint8_t *pRxData, uint16_t Size)
{
  /* 收发数据未完成 */
  txRxDataComplteFlag = RESET;
  
  /* 不使能SPI */
  LL_SPI_Disable(SPI1);
  
  /* 设置接收阈值为8bit */
  LL_SPI_SetRxFIFOThreshold(SPI1, LL_SPI_RX_FIFO_TH_QUARTER);
  
  /* 使能SPI的DMA接收中断 */
  LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_2);
  
  LL_DMA_ClearFlag_GI2(DMA1);
  LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_2, Size);
  uint32_t dataRegAddr = LL_SPI_DMA_GetRegAddr(SPI1);
  LL_DMA_ConfigAddresses(DMA1, LL_DMA_CHANNEL_2, dataRegAddr, (uint32_t)pRxData, LL_DMA_DIRECTION_PERIPH_TO_MEMORY);
  
  LL_DMA_DisableIT_HT(DMA1, LL_DMA_CHANNEL_2);
  LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_2);
  LL_DMA_EnableIT_TE(DMA1, LL_DMA_CHANNEL_2);
  
  LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_2);
  
  /* 使能SPI的DMA接收请求 */
  LL_SPI_EnableDMAReq_RX(SPI1);
  
  /* 使能SPI的DMA发送中断 */
  LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_1);
  
  LL_DMA_ClearFlag_GI1(DMA1);
  LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_1, Size);
  dataRegAddr = LL_SPI_DMA_GetRegAddr(SPI1);
  LL_DMA_ConfigAddresses(DMA1, LL_DMA_CHANNEL_1, (uint32_t)pTxData, dataRegAddr, LL_DMA_DIRECTION_MEMORY_TO_PERIPH);
  
  LL_DMA_DisableIT_HT(DMA1, LL_DMA_CHANNEL_1);
  LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_1);
  LL_DMA_EnableIT_TE(DMA1, LL_DMA_CHANNEL_1);
  
  LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_1);
  
  /* 使能SPI */
  LL_SPI_Enable(SPI1);
  
  /* 使能SPI的DMA发送请求 */
  LL_SPI_EnableDMAReq_TX(SPI1);
}

/*******************************************************************************
**功能描述 ：SPI的DMA发送回调函数
**输入参数 ：
**输出参数 ：
*******************************************************************************/
void APP_DmaSpiTxIRQCallback(void)
{
  if ((LL_DMA_IsActiveFlag_TC1(DMA1) == 1) && (LL_DMA_IsEnabledIT_TC(DMA1, LL_DMA_CHANNEL_1) == 1))
  {
    /* 关传输完成中断 */
    LL_DMA_DisableIT_TC(DMA1, LL_DMA_CHANNEL_1);
    LL_DMA_DisableIT_TE(DMA1, LL_DMA_CHANNEL_1);
    
    /* 清传输完成标志位 */
    LL_DMA_ClearFlag_TC1(DMA1);
  }
}

/*******************************************************************************
**功能描述 ：SPI的DMA接收回调函数
**输入参数 ：
**输出参数 ：
*******************************************************************************/
void APP_DmaSpiRxIRQCallback(void)
{
  if ((LL_DMA_IsActiveFlag_TC2(DMA1) == 1) && (LL_DMA_IsEnabledIT_TC(DMA1, LL_DMA_CHANNEL_2) == 1))
  {
    /* 关传输完成中断 */
    LL_DMA_DisableIT_TC(DMA1, LL_DMA_CHANNEL_2);
    LL_DMA_DisableIT_TE(DMA1, LL_DMA_CHANNEL_2);
    
    /* 清传输完成标志位 */
    LL_DMA_ClearFlag_TC2(DMA1);
    
    /* 等待传输完成 */
    while(LL_SPI_GetTxFIFOLevel(SPI1) != LL_SPI_TX_FIFO_EMPTY);
    while(LL_SPI_IsActiveFlag_BSY(SPI1) != 0);
    while(LL_SPI_GetRxFIFOLevel(SPI1) != LL_SPI_RX_FIFO_EMPTY);
    
    /* 屏蔽SPI的DMA发送和接收请求 */
    LL_SPI_DisableDMAReq_TX(SPI1);
    LL_SPI_DisableDMAReq_RX(SPI1);
    
    /* 置标志位，表面收发数据完成 */
    txRxDataComplteFlag = SET;
  }
}

/*******************************************************************************
**功能描述 ：SPI1等待传输完成，并校验数据
**输入参数 ：
**输出参数 ：
*******************************************************************************/
static void APP_WaitAndCheckEndOfTransfer(void)
{
  /* 1 - 等待传输结束 */
  while (txRxDataComplteFlag != SET)
  {
  }

  /* 2 - 比较发送数据和接收数据 */
  if(APP_Buffercmp8((uint8_t*)aTxBuffer, (uint8_t*)aRxBuffer, ubNbDataToTransmit))
  {
    /* 错误处理 */
    APP_LedBlinking();
  }
  else
  {
    /* 如果数据接收到，则打开 LED */
    BSP_LED_On(LED_GREEN);
  }
}

/*******************************************************************************
**功能描述 ：字符比较函数
**输入参数 ：pBuffer1：待比较缓冲区1
**           pBuffer2：待比较缓冲区2
**           BufferLength：待比较字符的个数
**输出参数 ：
*******************************************************************************/
static uint8_t APP_Buffercmp8(uint8_t* pBuffer1, uint8_t* pBuffer2, uint8_t BufferLength)
{
  while (BufferLength--)
  {
    if (*pBuffer1 != *pBuffer2)
    {
      return 1;
    }
    pBuffer1++;
    pBuffer2++;
  }

  return 0;
}

/*******************************************************************************
**功能描述 ：LED灯闪烁
**输入参数 ：
**输出参数 ：
*******************************************************************************/
static void APP_LedBlinking(void)
{
  while (1)
  {
    BSP_LED_Toggle(LED_GREEN);; 
    LL_mDelay(200);
  }
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
