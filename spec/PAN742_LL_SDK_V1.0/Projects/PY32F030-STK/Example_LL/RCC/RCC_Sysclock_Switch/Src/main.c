/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private define ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void SetSysclock(uint32_t SYSCLK);
void GPIO_Config(void);

/*******************************************************************************
**功能描述 ：执行函数
**输入参数 ：
**输出参数 ：
*******************************************************************************/
int main(void)
{

  /*初始化*/
  LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_SYSCFG);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  /*时钟初始化,配置系统时钟为LSI*/
  SystemClock_Config();

  /*引脚输出初始化*/
  GPIO_Config();
  
  /*MCO输出时钟及分频初始化*/
  LL_RCC_ConfigMCO(LL_RCC_MCO1SOURCE_SYSCLK,LL_RCC_MCO1_DIV_1);
  
  /*更换系统时钟为HSE*/
  SetSysclock(LL_RCC_SYS_CLKSOURCE_HSE);
  
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
  /* HSI使能及初始化 */
  LL_RCC_HSI_Enable();
  LL_RCC_HSI_SetCalibFreq(LL_RCC_HSICALIBRATION_24MHz);
  while(LL_RCC_HSI_IsReady() != 1)
  {
  }
  
  /* LSI使能及初始化 */
  LL_RCC_LSI_Enable();
  while(LL_RCC_LSI_IsReady() != 1)
  {
  }
  
  /* HSE使能及初始化 */
  LL_RCC_HSE_Enable();
  LL_RCC_HSE_SetFreqRegion(LL_RCC_HSE_16_32MHz);
  while(LL_RCC_HSE_IsReady() != 1)
  {
  }
  
  /* 设置AHB分频*/
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);

  /* 配置LSI为系统时钟及初始化 */
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_LSI);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_LSI)
  {
  }

  /*设置APB1分频及初始化*/
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_Init1msTick(LSI_VALUE);
  /* 更新系统时钟全局变量SystemCoreClock(也可以通过调用SystemCoreClockUpdate函数更新) */
  LL_SetSystemCoreClock(LSI_VALUE);
}

/*******************************************************************************
**功能描述 ：配置HSE为系统时钟
**输入参数 ：
**输出参数 ：
*******************************************************************************/
void SetSysclock(uint32_t SYSCLK)
{
  /* 配置HSE为系统时钟及初始化 */
  LL_RCC_SetSysClkSource(SYSCLK);
  while(LL_RCC_GetSysClkSource() != SYSCLK)
  {
  }
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
